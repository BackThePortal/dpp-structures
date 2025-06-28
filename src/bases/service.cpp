//
// Created by backo on 17/06/25.
//

#include "service.h"
#include "utilities/text.h"
#include <array>
#include <iostream>
#include <subprocess.h>
#include <utility>

using namespace dpp_structures;

std::string arguments::join() const { return dpp_structures::join(this->list(), " "); }

std::vector<std::string> arguments::list() const {
	std::vector<std::string> v{this->main};

	for (const auto& flag: this->flags) v.push_back(flag);

	char options_sep = this->args_type;

	for (const auto& option: options) { v.push_back('-' + option.first + options_sep + option.second); }

	return v;
}
/*
service::execution_result service::start(const arguments& args) {
	std::array<char, 128> buffer{};
	execution_result result;

	std::string command = this->executable_path + " " + args.join();

	subprocess::popen cmd(command);

	std::unique_ptr<FILE, std::function<void(FILE*)>> pipe(popen(command, "r"))
}*/
service::service(std::string executable_path) : executable_path(std::move(executable_path)) {}

void service::exec(const arguments& args, std::function<void(execution_result)> callback) {
	dpp::utility::exec(this->executable_path, args.list(),
					   [callback = std::move(callback)](int exit_code, const std::string& output) {
						   callback({output, exit_code});
					   });
	/*
	subprocess::popen subprocess(this->executable_path, argsList);
	execution_result result;

	result.exit_code = subprocess.wait();

	std::string line;
	while(std::getline(subprocess.err(), line)) {
		result.output += line + '\n';
	}

	return result;
	*/
}
dpp::task<service::execution_result> service::run(arguments args) {
	co_return co_await dpp::async<execution_result>(
			[this, args = std::move(args)](std::function<void(execution_result)> callback) {
				this->exec(args, std::move(callback));
			});
}
