#if !defined(AST_ADAPTED_HPP)
#define AST_ADAPTED_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(clvl::ast::infinitive,
    infinitive_spanish, infinitive_english
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::mood,
    mood_spanish, mood_english
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::tense,
    tense_spanish, tense_english
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::verb_english,
    verb_english_example
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::form,
    form_1s, form_2s, form_3s, form_1p, form_2p, form_3p
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::alt_form,
    form_2s, form_3s, form_2p, form_3p
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::alt_form2,
    form_3s, form_3p
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::gerund,
    gerund_spanish, gerund_english
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::pastparticiple,
    pastparticiple_spanish, pastparticiple_english
)
BOOST_FUSION_ADAPT_STRUCT(clvl::ast::verb, infinitive_, mood_, tense_, verb_english_, forms_, gerund_, pastparticiple_)

#endif
