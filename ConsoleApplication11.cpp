#include "pch.h"  
#include <iostream>
#include <cctype>  
#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <fstream>


using namespace std;

struct Node {
	virtual int Evaluate() const = 0;
};

struct Value : public Node {
	Value(char digit) : _value(digit - '0') {}

	int Evaluate() const override { return _value; }

private:
	const uint8_t _value;
};

struct Variable : public Node {
	Variable(char x) : _x(x - '0') {}

	int Evaluate() const override { return _x; }

private:
	const uint8_t _x;
};

struct Op : public Node {
	Op(char value)
		: precedence([value] {
		if (value == '/') {
			return 3;
		}

		else if (value == '*') {
			return 2;
		}
		else {
			return 1;
		}
	}()),
		_op(value) {}

	const uint8_t precedence;

	int Evaluate() const override {
		 if (_op == '/') {
		return _left->Evaluate() / _right->Evaluate();
		}
		else if (_op == '*') {
			return _left->Evaluate() * _right->Evaluate();
		}
		else if (_op == '+') {
			return _left->Evaluate() + _right->Evaluate();
		}
		else if (_op == '-') {
			return _left->Evaluate() - _right->Evaluate();
		}

		return 0;
	}

	void SetLeft(shared_ptr<Node> node) { _left = node; }
	void SetRight(shared_ptr<Node> node) { _right = node; }

private:
	const char _op;
	shared_ptr<const Node> _left, _right;
};

template <class Iterator>
shared_ptr<Node> Parse(Iterator token, Iterator end, const int &x) {
	
	if (token == end) {
		return make_shared<Value>('0');
	}

	stack<shared_ptr<Node>> values;
	stack<shared_ptr<Op>> ops;
	int _x;

	auto PopOps = [&](int precedence) {
		while (!ops.empty() && ops.top()->precedence >= precedence) {
			auto value1 = values.top();
			values.pop();
			auto value2 = values.top();
			values.pop();
			auto op = ops.top();
			ops.pop();

			op->SetRight(value1);
			op->SetLeft(value2);

			values.push(op);
		}
	};

	while (token != end) {


		const auto &value = *token;
		if (value == 'I') {
			++token;
			const auto &value = *token;
			_x = value;
			

		}
		else if (value >= '0' && value <= '9' ) {
			values.push(make_shared<Value>(value));
		}
		else if (value == 'x') {
			values.push(make_shared<Variable>(_x));
		}
		else if (value == '/') {
			PopOps(3);
			ops.push(make_shared<Op>(value));
		}
		else if (value == '*') {
			PopOps(2);
			ops.push(make_shared<Op>(value));
		}
		else if (value == '+' || value == '-') {
			PopOps(1);
			ops.push(make_shared<Op>(value));
		}
		++token;
	}

	while (!ops.empty()) {
		PopOps(0);
	}

	return values.top();
}

int main() {
	setlocale(LC_ALL, "rus");
	string tokens;
	cout << "xI5*2+1+34/x" << endl;
	cout << "x - number, I - separator" << endl;

	
	ifstream fin("parse.txt"); 
	string s;
	if (!fin.is_open()) 
		cout << "Файл не может быть открыт!\n"; 
	else
	{
		char nil = '\0';
		getline(fin, s, nil);
		fin.close();

	}

	int x = 0;
	auto node = Parse(s.begin(), s.end(), x);
	
	
	cout << "Expression value: " << node->Evaluate() << endl;
	system("pause");

	return 0;
}