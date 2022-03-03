#if !defined(AST_HPP)
#define AST_HPP

#include <string>

namespace clvl { namespace ast
{
    namespace x3 = boost::spirit::x3;
    struct infinitive : x3::position_tagged
    {
        infinitive(
            std::string const& infinitive_spanish = ""
          , std::string const& infinitive_english = ""
        )
          : infinitive_spanish(infinitive_spanish)
          , infinitive_english(infinitive_english)
        {}
        std::string infinitive_spanish, infinitive_english;
    };
    struct mood : x3::position_tagged
    {
        mood(
            std::string const& mood_spanish = ""
          , std::string const& mood_english = ""
        )
          : mood_spanish(mood_spanish)
          , mood_english(mood_english)
        {}
        std::string mood_spanish, mood_english;
    };
    struct tense : x3::position_tagged
    {
        tense(
            std::string const& tense_spanish = ""
          , std::string const& tense_english = ""
        )
          : tense_spanish(tense_spanish)
          , tense_english(tense_english)
        {}
        std::string tense_spanish, tense_english;
    };
    struct verb_english : x3::position_tagged
    {
        verb_english(
            std::string const& verb_english_example = ""
        )
          : verb_english_example(verb_english_example)
        {}
        std::string verb_english_example;
    };
    struct form : x3::position_tagged
    {
        form(
            std::string const& form_1s = ""
            , std::string const& form_2s = ""
            , std::string const& form_3s = ""
            , std::string const& form_1p = ""
            , std::string const& form_2p = ""
            , std::string const& form_3p = ""
        )
          : form_1s(form_1s)
	  , form_2s(form_2s)
	  , form_3s(form_3s)
	  , form_1p(form_1p)
	  , form_2p(form_2p)
	  , form_3p(form_3s)
        {}
        std::string form_1s, form_2s, form_3s, form_1p, form_2p, form_3p;
    };
    struct alt_form : x3::position_tagged
    {
        alt_form(
            std::string const& form_2s = ""
            , std::string const& form_3s = ""
            , std::string const& form_2p = ""
            , std::string const& form_3p = ""
        )
          : form_2s(form_2s)
	  , form_3s(form_3s)
	  , form_2p(form_2p)
	  , form_3p(form_3s)
        {}
        std::string form_2s, form_3s, form_2p, form_3p;
    };
    struct alt_form2 : x3::position_tagged
    {
        alt_form2(
            std::string const& form_3s = ""
            , std::string const& form_3p = ""
        )
          : form_3s(form_3s)
	  , form_3p(form_3s)
        {}
        std::string form_3s, form_3p;
    };
    struct gerund : x3::position_tagged
    {
        gerund(
            std::string const& gerund_spanish = ""
          , std::string const& gerund_english = ""
        )
          : gerund_spanish(gerund_spanish)
          , gerund_english(gerund_english)
        {}
        std::string gerund_spanish, gerund_english;
    };
    struct pastparticiple : x3::position_tagged
    {
        pastparticiple(
            std::string const& pastparticiple_spanish = ""
          , std::string const& pastparticiple_english = ""
        )
          : pastparticiple_spanish(pastparticiple_spanish)
          , pastparticiple_english(pastparticiple_english)
        {}
        std::string pastparticiple_spanish, pastparticiple_english;
    };
    using forms = boost::variant<form, alt_form, alt_form2>;
    struct verb : x3::position_tagged
    {
        infinitive infinitive_;
        mood mood_;
        tense tense_;
        verb_english verb_english_;
	    forms forms_;
        gerund gerund_;
        pastparticiple pastparticiple_;
    };
    using boost::fusion::operator<<;
}}

#endif
