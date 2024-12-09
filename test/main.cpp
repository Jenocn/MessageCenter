
#include <Dispatcher.h>
#include <iostream>

int main(int, char**) {

	int sender;

	Message::Dispatcher dispatcher;
	dispatcher.AddListener<int>(&sender, [](const int& msg) {
		std::cout << "message: " << msg << std::endl;
	});

	dispatcher.Send(10);

	dispatcher.RemoveListener<int>(&sender);

	dispatcher.Send(10);

	return 0;
}