#ifndef EXPRESSION_H

#define EXPRESSION_H

class Expression
{
public:
	virtual ~Expression()
	{
	}

	virtual unsigned long long evaluate() = 0;
};

#endif // !EXPRESSION_H