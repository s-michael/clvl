#include <boost/program_options.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>
#include <boost/variant.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <iostream>
#include <algorithm>
#include "ast_adapted.hpp"

namespace clvl
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;

        struct position_cache_tag;

        struct annotate_position
        {
            template <typename T, typename Iterator, typename Context>
            inline void on_success(Iterator const& first, Iterator const& last
            , T& ast, Context const& context)
            {
                auto& position_cache = x3::get<position_cache_tag>(context).get();
                position_cache.annotate(ast, first, last);
            }
        };

        struct error_handler
	{
		template <typename Iterator, typename Exception, typename Context>
		x3::error_handler_result on_error(
			Iterator& first, Iterator const& last,
			Exception const& x, Context const& context)
			{
				auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
				std::string message = "Error.  Expecting: " + x.which() + " here: ";
				error_handler(x.where(), message);
				return x3::error_handler_result::fail;
			}
	};

	using x3::lexeme;
	using x3::char_;
	using x3::lit;
	using x3::space;

	struct quoted_string_class;
	struct infinitive_class;
	struct mood_class;
	struct tense_class;
	struct verb_english_class;
	struct form_class;
	struct alt_form_class;
	struct alt_form2_class;
	struct gerund_class;
	struct pastparticiple_class;
	struct verb_class;

	x3::rule<quoted_string_class, std::string> const quoted_string = "quoted_string";
	x3::rule<infinitive_class, ast::infinitive>  const infinitive = "infinitive";
	x3::rule<mood_class, ast::mood>  const mood = "mood";
	x3::rule<verb_class, ast::verb> const verb = "verb";
	x3::rule<tense_class, ast::tense>  const tense = "tense";
	x3::rule<verb_english_class, ast::verb_english> const verb_english = "verb_english";
	x3::rule<form_class, ast::form> const form = "form";
	x3::rule<alt_form_class, ast::alt_form> const alt_form = "alt_form";
	x3::rule<alt_form2_class, ast::alt_form2> const alt_form2 = "alt_form2";
	x3::rule<gerund_class, ast::gerund> const gerund = "gerund";
	x3::rule<pastparticiple_class, ast::pastparticiple> const pastparticiple = "pastparticiple";

	auto const quoted_string_def =  lexeme['"' >> +(char_ - '"') >> '"']; 
	auto const quoted_empty  = lexeme[lit('"') > lit('"')];
	auto const infinitive_def =  quoted_string > ',' > quoted_string;
	auto const mood_def =  quoted_string > ',' > quoted_string;
	auto const tense_def =  quoted_string > ',' > quoted_string;
	auto const verb_english_def =  quoted_string;
	auto const form_def =  quoted_string > ',' > quoted_string > ',' > quoted_string> ',' > quoted_string > ',' > quoted_string > ',' > quoted_string;
	auto const alt_form_def =  quoted_empty > ',' > quoted_string > ',' > quoted_string > ',' > quoted_empty > ',' > quoted_string > ',' > quoted_string;
	auto const alt_form2_def =  quoted_empty > ',' > quoted_empty > ',' > quoted_string > ',' > quoted_empty > ',' > quoted_empty > ',' > quoted_string;
	auto const gerund_def =  quoted_string > ',' > quoted_string;
	auto const pastparticiple_def =  quoted_string > ',' > quoted_string;

	auto const forms = form | alt_form | alt_form2; 

	auto const verb_def =
	       infinitive
	       > ','
	       > mood
	       > ','
	       > tense
	       > ','
	       > verb_english
	       > ','
	       > forms
	       > ','
	       > gerund
	       > ','
	       > pastparticiple
	       ;

	auto const verbs =  verb >> *(verb); 

        BOOST_SPIRIT_DEFINE(quoted_string, infinitive, mood, tense, verb_english, form, alt_form, alt_form2, gerund, pastparticiple, verb);

        struct quoted_string_class {}; 
        struct infinitive_class : annotate_position {}; 
        struct mood_class : annotate_position {}; 
        struct tense_class : annotate_position {}; 
        struct verb_english_class : annotate_position {}; 
        struct form_class : annotate_position {};
        struct alt_form_class : annotate_position {};
        struct alt_form2_class : annotate_position {};
        struct gerund_class : annotate_position {}; 
        struct pastparticiple_class : annotate_position {}; 
        struct verb_class : error_handler, annotate_position {};
    }
}

using iterator_type = boost::iostreams::mapped_file::const_iterator;
using position_cache = boost::spirit::x3::position_cache<std::vector<iterator_type>>;

std::vector<clvl::ast::verb>
parse(boost::iostreams::mapped_file mmap, position_cache& positions)
{
    	namespace spirit = boost::spirit;
    	namespace x3 = spirit::x3;

    	using x3::ascii::space;
	using x3::with;
	using x3::lexeme;
	using x3::char_;
	using x3::string;
	using x3::lit;

    	iterator_type iter = mmap.const_data();
    	iterator_type const end = mmap.const_data() + mmap.size();
	std::vector<clvl::ast::verb> ast;
    	using clvl::parser::verbs;
    	using clvl::parser::position_cache_tag;
	using boost::spirit::x3::error_handler_tag;
    	using error_handler_type = boost::spirit::x3::error_handler<iterator_type>;

    	error_handler_type error_handler(iter, end, std::cerr);

	auto const parser = 
        with<position_cache_tag>(std::ref(positions))
        [
		with<error_handler_tag>(std::ref(error_handler))
		[
			verbs
		]
        ];

	bool r = phrase_parse(iter, end, parser, space, ast);
	if(r && iter == end)
		return ast;

	else {
		std::cout << "Parsing failed\n";
		ast.clear();
	}
	return ast;
}

struct forms_visitor : public boost::static_visitor<> 
{
	void operator()(clvl::ast::form& f) const { 
		std::cout << boost::format("%1% %2% %3% %4% %5% %6%") 
			% f.form_1s 
			% f.form_2s 
			% f.form_3s 
			% f.form_1p 
			% f.form_2p 
			% f.form_3p 
			<< 
			std::endl;
	}
	void operator()(clvl::ast::alt_form& af) const { 
		std::cout << boost::format("%1% %2% %3% %4%") 
			% af.form_2s 
			% af.form_3s 
			% af.form_2p 
			% af.form_3p 
			<< 
			std::endl;
	}
	void operator()(clvl::ast::alt_form2& af2) const { 
		std::cout << boost::format("%1% %2%") 
			% af2.form_3s 
			% af2.form_3p 
			<< 
			std::endl;
	}
};

int main(int argc, const char *argv[])
{
   	namespace io = boost::iostreams;
        namespace fs = boost::filesystem;
   	namespace x3 = boost::spirit::x3;
	namespace opt = boost::program_options;

  	try
  	{ 
		std::string help_desc = R"(
Usage: clvl verb 
   or: clvl verb [OPTION]...

Search for a Spanish verb and return the conjugation in English.
Buscar un verbo en español y devolver la conjugación en inglés.

Example:  clvl comer
Example2: clvl comer "Futuro perfecto"
Example3: clvl comer "Futuro perfecto" "Subjuntivo"
Example4: clvl comer "Futuro perfecto" "Subjuntivo" --file data/my_conjugation_file.csv 
OPTIONS)";
		std::string file_desc = R"(
A CSV formatted data file to parse. Defaults to ../data/jehle_verb_database.csv)";
		std::string mood_desc = R"(
The mood with which to conjugate the verb.  Defaults to 'Indicativo'.
Moods are as follows:
  "Indicativo"
  "Subjuntivo"
)";
		std::string tense_desc = R"(
The tense with which to conjugate the verb.  Defaults to 'Presente'.
Tenses are as follows:
  "Presente"
  "Futuro"
  "Imperfecto"
  "Pretérito"
  "Condicional"
  "Presente perfecto"
  "Futuro perfecto"
  "Pluscuamperfecto"
  "Pretérito anterior"
  "Condicional perfecto"

)";
		std::string verb_desc = R"(
A Spanish verb in infinitive form e.g. 'comer' to be conjugated.)";

		opt::options_description desc{help_desc};
    	  	desc.add_options()
      		("help,h", "Help screen")
      		("file,f", opt::value<std::string>()->default_value("../data/jehle_verb_database.csv"), file_desc.data())
      		("tense,t", opt::value<std::string>()->default_value("Presente"), tense_desc.data())
      		("mood,m", opt::value<std::string>()->default_value("Indicativo"), mood_desc.data())
      		("verb,v", opt::value<std::string>()->required(), verb_desc.data());

		opt::positional_options_description pos_desc;
		pos_desc.add("verb",1);
		pos_desc.add("tense",1);
		pos_desc.add("mood",1);
		opt::command_line_parser parser{argc, argv};
		parser.options(desc).positional(pos_desc).allow_unregistered();
		opt::parsed_options parsed_options = parser.run();

		opt::variables_map vm;
		opt::store(parsed_options, vm);

		if (vm.count("help"))
		{
			std::cout << desc << std::endl;
			return EXIT_FAILURE;
		}

		opt::notify(vm);

		std::string const file = vm["file"].as<std::string>();

		try 
		{
			auto const size = fs::file_size(file);
			if(!size)
			{
				std::cout << "Usage: clvl <csv formatted data file> <search_term>" << std::endl;
				std::cout << "The data file appears to be empty." << std::endl;
				return EXIT_FAILURE;
			}
			if(size)
			{
				// check first line of file for correct format - to save time loading a large file that cannot be parsed
				std::ifstream infile(file);
				std::string line;
				std::getline(infile, line);
				bool r = x3::parse(line.begin(), line.end(), clvl::parser::quoted_string % ',');
				if(r)
				{
					infile.close();
				} 
				else {
					std::cout << "Usage: clvl <csv formatted data file> <search_term>" << std::endl;
					std::cout << "The data file appears to be incorrectly formatted: CSV with quoted strings." << "e.g: " << std::endl;
					std::cout << "infinitive"
						<< "," << "infinitive_english"
						<< "," << "mood" 
						<< "," << "mood_english"
						<< "," << "tense"
						<< "," << "tense_english"
						<< "," << "verb_english"
						<< "," << "form_1s"
						<< "," << "form_2s"
						<< "," << "form_3s" 
						<< "," << "form_1p"
						<< "," << "form_2p"
						<< "," << "form_3p"
						<< "," << "gerund"
						<< "," << "gerund_english"
						<< "," << "pastparticiple"
						<< "," << "pastparticiple_english"
						<< std::endl;
						
					return EXIT_FAILURE;
				}
			}
		} catch(const fs::filesystem_error& ex)
		{ 
			std::cout << ex.what() << std::endl;
			std::cout << "Usage: clvl <csv formatted data file> <search_term>" << std::endl;
			std::cout << "The data file appears to be missing or corrupt." << std::endl;
			return EXIT_FAILURE;
		}

		boost::iostreams::mapped_file mmap(file, boost::iostreams::mapped_file::readonly);

		position_cache positions{mmap.begin(), mmap.end()};

                std::vector<clvl::ast::verb> result = parse(mmap, positions);

		std::string const search_term = vm["verb"].as<std::string>();
		std::string const tense = vm["tense"].as<std::string>();
		std::string const mood = vm["mood"].as<std::string>();

		auto pred = [s = search_term, t = tense, m = mood](const clvl::ast::verb& v)
				{ return (v.infinitive_.infinitive_spanish == s && v.tense_.tense_spanish == t && v.mood_.mood_spanish == m); };

		auto it = std::find_if(result.begin(), result.end()
				, pred);

		if (it != result.end())
		{
			std::cout << boost::format("%1% %2% %3% %4% %5% %6% %7%")
				% it->infinitive_.infinitive_spanish
				% it->infinitive_.infinitive_english
				% it->mood_.mood_spanish
				% it->tense_.tense_spanish
				% it->verb_english_.verb_english_example
				% it->gerund_.gerund_spanish
				% it->pastparticiple_.pastparticiple_spanish
				<< std::endl;
			boost::apply_visitor(forms_visitor(), it->forms_);
		} else
			std::cout << "Could not find: " << search_term << std::endl;

		return EXIT_SUCCESS;
  	}
  	catch (const opt::error &ex)
  	{
    		std::cerr << ex.what() << std::endl;
  	}
}
