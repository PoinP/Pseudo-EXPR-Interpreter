#include "../doctest.h"

#include "../../src/Utility/Helper.h"

#include "../../src/Token/Tokenizer.h"
#include "../../src/Parser/Parser.h"


TEST_CASE("Usage Of A Variable Which Is Not Initalized")
{
	Tokenizer t("x = y");
	Environment env;
	Parser p(t.getTokens(), &env);

	CHECK_THROWS_WITH_AS(p.parse()[0]->run(), "Run-Time Error: { The variable \"y\" is not initalized } at line 1", RunTimeError);
}

TEST_CASE("Usage Of An Undefined Function")
{
	Tokenizer t("x = FUNC[x]");
	Environment env;
	Parser p(t.getTokens(), &env);

	CHECK_THROWS_WITH_AS(p.parse()[0]->run(), "Run-Time Error: { Function \"FUNC\" undefined } at line 1", RunTimeError);
}