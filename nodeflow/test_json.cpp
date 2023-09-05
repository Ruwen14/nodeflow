#include "pch.h"
#include "reflection/type_reflection.hpp"
#include "core/UUID.hpp"


struct Person
{

	char name[50];
	int age;
	char phone[24];
};


using namespace std;
using json = nlohmann::json;
int main()
{
	std::ofstream scriptF("MyScript.fscript");
	json j; 

	// meta
	j["meta"]["version"] = "0.0.1";
	j["meta"]["moduleID"] = static_cast<std::uint64_t>(nf::UUID::create());

	// nodes
	for (size_t i = 0; i < 30; i++)
		j["nodes"].push_back(static_cast<std::uint64_t>(nf::UUID::create()));
	
	for (auto& jNodeID : j["nodes"])
	{
		auto nodeID = jNodeID.get<std::uint64_t>();
		auto& node = j[std::to_string(nodeID)];
		
		node["name"] = "WhileLoop";
		node["elementype"] = "Node";
		node["archetype"] = "FlowNode";
		node["guid"] = static_cast<std::uint64_t>(nf::UUID::create());
		node["inPortCount"] = 0;
		node["outPortCount"] = 2;
		node["userEditorData"] = nullptr;
		node["userCustomData"] = nullptr;

		auto& outPorts = node["nodePorts"]["out"];
		
		for (size_t i = 0; i < 3; i++)
		{
			auto portOut = json::object();

			portOut["portIndex"] = i;
			portOut["name"] = "Target";
			auto& portOutConnections = portOut["connections"];

			for (size_t i = 0; i < 2; i++)
			{
				portOutConnections.push_back(json::object_t::value_type("toNode", (std::uint64_t)nf::UUID::create()));
				portOutConnections.push_back(json::object_t::value_type("toPort", i));
			}
			dbgln(portOutConnections.size());


			outPorts.push_back(portOut);
		}


	}

	scriptF << j.dump(4);
	exit(0);


	


	int a = 3;
	dbgln(nf::type_id<Person>());
	dbgln(nf::type_id<int>());
	dbgln(12897946762296273637 == 12897946762296273637);

	size_t fmVersionHash = 30;
	std::ifstream scriptFile("dummy.fs");
	try {
// 		auto j = nlohmann::json::parse(scriptFile);
		fmVersionHash = j["meta"]["flowModuleVersionHash"].get<size_t>();
		dbgln("Version Hash is: {}", fmVersionHash);
	}
	catch (std::exception& e)
	{
		dbgln(e.what());
	}
	return 0;
}
