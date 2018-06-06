#include <sstream>
#include<iostream>
#include<string>

template <typename T>
class tree_t
{
private:
	struct node_t {
		node_t * left;
		node_t * right;
		T value;
	};
private:
	node_t * root_;
public:
	tree_t()
	{
		root_= nullptr;
	}
	void del(node_t * node)
	{	
		if (root_ != nullptr) {
			if (node->left) del (node->left);
			if (node->right) del (node->right);
			delete node;
		}
	}
	~tree_t()
	{
			del(root_);
			//root_ = nullptr;
	}
	
	bool Empty()
	{
		return (!root_);
	}
	
bool insert(T value)
{
	if (find(value) == true) {
		return false;
	}
	else
	{
		node_t * node = new node_t;
		node->value = value;
		node->left = nullptr;
		node->right = nullptr;

		if (root_ == nullptr) {
			root_ = node;
		}
		else {
			node_t * parrent = nullptr;
			node_t * temp = root_;
			while (temp != nullptr) {
				parrent = temp;
				if (value > temp->value) {
					temp = temp->right;
					parrent->right=node;
				}
				else if (value < temp->value)
				{
					temp = temp->left;
					parrent->left=node;
				}
				
			}
			return true;
		}
	}
}
	
bool find(T value) const
	{
		node_t * temp = root_;
		while (temp != nullptr){
			if (value == temp->value) {
				return true;
			}
			else {
				if (value > temp->value) {
					temp = temp->right;
				}
				else {
					temp = temp->left;
				}
			}
		}
		return false;
	}
	
	node_t * root() const
	{
		return  root_;
	}
	void print(std::ostream & stream ,  node_t * node , size_t i = 1) const
	{
		if (node->right) {
			i++;
			print(stream ,node->right, i);
			i--;
		}
		for (size_t k = 0; k < i; k++) {
			stream << "--";
		}
			stream << node->value << std::endl;
		if (node->left) {
			i++;
			print(stream ,node->left, i);
			i--;
		}
	}
	
	void CheckOperator(char op,  T value = 0 , std::ostream &stream = std::cout )
	{
		if (op != '=')
		{
			switch (op) {
			case '+': {
				insert(value);
				break;
			}
			case '?': {
				if (find(value)) {
					stream << "true";
				}
				else stream << "false";
				break;
			}
			default: {
				if (op != 'q' ) throw std::invalid_argument("Fail");
				break;
			}
			}
		}
		else {
			print(stream, root_);
		}
	}
	
	tree_t(std::initializer_list<T> keys)
	{
		root_ = nullptr;
		size_t size = keys.size();
		for (auto& i : size) {	
			T a = *(keys.begin()+i);
			insert(a);
		}
	}
	
	bool equal(node_t * lhs, node_t * rhs) const
	{
		if (!lhs && !rhs) {
			return true;
		}
		else if (lhs && rhs && lhs->value == rhs->value) {
			return equal(lhs->left, rhs->left) && equal(lhs->right, rhs->right);
		}
		else {
			return false;
		}
	}

	auto operator==(tree_t const & other) const
	{
		return(equal(root_, other.root()));
	}
	bool remove(T key)
	{
		node_t ** link, *temp;
		link = &root_;
		temp = root_;
		for (;;)
		{
			if (temp == nullptr){
				return false;
			}
			else if (key == temp->value){
				break;
			}
			else if (key > temp->value){
				link = &temp->right;
				temp = temp->right;
			}
			else{
				link = &temp->left;
				temp = temp->left;
			}
		}

		if (temp->right == nullptr) {
			*link = temp->left;
		}
		else{
			node_t * temp_right = temp->right;
			if (temp_right->left == nullptr){
				temp_right->left = temp->left;
				*link = temp_right;
			}
			else{
				node_t * temp_left_of_right = temp_right->left;
				while (temp_left_of_right->left != nullptr){
					temp_right = temp_left_of_right;
					temp_left_of_right = temp_right->left;
				}
				temp_right->left = temp_left_of_right->right;
				temp_left_of_right->left = temp->left;
				temp_left_of_right->right = temp->right;
				*link = temp_left_of_right;
			}
		}
		delete temp;
		return true;
	}
};

template <typename T>
bool read(char & op, T & value , bool & fail)
{
	
	std::string line;
	getline(std::cin, line);
	std::istringstream stream(line);
	if (stream >> op) {
		if ( (op == '+' && stream >> value) || (op == '?' && stream >> value)) {
			return true;
		}
		else if (op == '=' ) {
			return true;
		}
		else if(op == 'q') {
			return false;
		}
	}
	fail = true;
	return false;

}

template <typename T>
void Operator(std ::ostringstream &stream , tree_t<T> & tree)
{
	
	char op;
	T value;
	bool fail=false;
	while (read(op, value, fail)) {
		if (op == '=') {
			tree.print(stream, tree.root());
			std::cout << stream.str();
		}
		else {
			if (op == '+') tree.insert(value);
			if (op == '?') tree.find(value);
		}
	}
	if (fail) std::cerr << "fail";
}
