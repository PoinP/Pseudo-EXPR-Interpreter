#include "doctest.h"

#include "../src/Interpreter/Environmnet.h"
#include "../src/Parser/Expressions/Primitive.h"


TEST_CASE("Construtors")
{
	SUBCASE("Default Ctor")
	{
		Environment env;

		CHECK(env.contains("a") == false);
		CHECK_THROWS(env.get("a"));
	}

	SUBCASE("Another env as Ctor")
	{
		Environment outerEnv;
		outerEnv.set("var", new Primitive((BigInteger)12));

		Environment currentEnv(&outerEnv);

		CHECK(currentEnv.contains("var") == true);
		CHECK_NOTHROW(currentEnv.get("var"));
	}
}

TEST_CASE("Set Tests")
{
	Environment env;

	SUBCASE("Setting a non existent key")
	{
		env.set("var", new Primitive((BigInteger)12));
		CHECK(env.contains("var") == true);
		CHECK(env.get("var")->evaluate(&env) == 12);
	}

	SUBCASE("Re-setting a variable")
	{
		env.set("var", new Primitive((BigInteger)12));
		CHECK(env.contains("var") == true);
		CHECK(env.get("var")->evaluate(&env) == 12);

		env.set("var", new Primitive((BigInteger)99));
		CHECK(env.contains("var") == true);
		CHECK(env.get("var")->evaluate(&env) == 99);
	}

	SUBCASE("Setting a variable on self")
	{
		env.set("var", new Primitive((BigInteger)12));

		Environment outerEnv(&env);
		outerEnv.setOnSelf("var", new Primitive((BigInteger)99));

		CHECK(env.get("var")->evaluate(&env) == 12);
		CHECK(outerEnv.get("var")->evaluate(&outerEnv) == 99);

		outerEnv.setOnSelf("var", new Primitive(77));
		CHECK(outerEnv.get("var")->evaluate(&outerEnv) == 77);
	}
}

TEST_CASE("Contains Tests")
{
	Environment env;

	SUBCASE("Doesn't contain a variable")
	{
		CHECK(env.contains("var") == false);
		CHECK_THROWS(env.get("var"));
	}

	SUBCASE("Contains a variable")
	{
		env.set("var", new Primitive((BigInteger)12));
		CHECK(env.contains("var") == true);
		CHECK_NOTHROW(env.get("var"));
	}

	SUBCASE("Contains a variable in upper environment")
	{
		env.set("var", new Primitive((BigInteger)12));

		Environment subEnv(&env);
		CHECK(env.contains("var") == true);
		CHECK_NOTHROW(env.get("var"));
	}
}

TEST_CASE("Get Tests")
{
	Environment env;

	SUBCASE("Nonexistant variable")
	{
		CHECK_THROWS(env.get("var"));
	}

	SUBCASE("Existing variable")
	{
		env.set("var", new Primitive((BigInteger)12));
		CHECK_NOTHROW(env.get("var"));
		CHECK(env.get("var")->evaluate(&env) == 12);
	}

	SUBCASE("Exisiting variable in upper environment")
	{
		env.set("var", new Primitive((BigInteger)12));

		Environment subEnv(&env);
		CHECK_NOTHROW(subEnv.get("var"));
		CHECK(subEnv.get("var")->evaluate(&subEnv) == 12);
	}
}