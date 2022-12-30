#ifndef EXPRESSION_H

#define EXPRESSION_H

class Expression
{
public:
	virtual ~Expression()
	{
	}

	virtual unsigned long long evaluate() const = 0;
};

#endif // !EXPRESSION_H
