// #include "core/NFAbstractNodeModel.hpp"

#include "core/type_tricks.hpp"

#include "../3rdparty/cpputils/prettyprint.h"
#include "../3rdparty/nlohmann/json.hpp"
#include "../3rdparty/portable-file-dialogs/portable-file-dialogs.h"

#include <fstream>

#include <windows.h>


using namespace cpputils;

// #ifdef _WIN32
// #pragma comment(lib, "liblua54.a")
// #endif







#include "archive/CodeGen.hpp"

#include "archive/Reflection.hpp"

namespace nf
{
	struct Serializer
	{
		uint64_t id;
		std::function<void(std::ostringstream&, void*)> func;
	};

	template<typename T>
	Serializer MakeSerializer()
	{
		auto lambda = [](std::ostringstream& stream, void* data) {
			stream << *static_cast<T*>(data);
		};

		return { entt::type_hash<T>::value(), lambda };
	}
}

// using Serializer = std::function<void(std::stringstream& archive, void* data)>;
// 
// template<typename T>
// Serializer GenerateSerializer()
// {
// 	static_assert(nf::has_ostream_operator_v<T>, "No << operator found.");
// 	auto lambda = [](std::stringstream& archive, void* data) {
// 		archive << *static_cast<T*>(data);
// 	};
// 
// 	return lambda;
// }



struct CPPCodeInstruction
{
	enum InstructionType
	{
		VariableInitialization,
		VariableSet,
		VariableGet,
		FreeFunctionCall,
		MemberFunctionCall
	};

	template<auto func, bool e = false>
	static CPPCodeInstruction GenCallInstruction_FreeFunction()
	{
		using codeContext = nf::FunctionCodeContext<func>;
		using retType = codeContext::ReturnType;

		CPPCodeInstruction ci;
		ci.instructionType = CPPCodeInstruction::FreeFunctionCall;

		std::stringstream stream;
		if constexpr (!std::is_same_v<retType, void>)
		{
			stream << codeContext::ReturnTypeName() << " $r1 = ";
		}

		stream << codeContext::FunctionName() << "(";

		if constexpr (constexpr int argCount = codeContext::ArgumentCount() != 0)
		{
			ci.functionArgumentCount = argCount;
			auto argNames = codeContext::ArgumentTypeNames();

			for (size_t i = 0; i < argNames.size(); i++)
			{
				stream << "$a" << i + 1;

				if (i != argNames.size() - 1)
				{
					stream << ", ";

				}
			}
		}

		stream << ");";


		ci.instruction = stream.str();
		return ci;
	}

	InstructionType instructionType;
	int functionArgumentCount = -1;
	std::string instruction;
};




#include "core/NodePort.hpp"
#include "script/FlowModule.hpp"
#include "core/Node.hpp"
#include "core/Error.hpp"
#include "../3rdparty/cpputils/timer.h"
#include "utility/Expected.hpp"

#include "nodes/FunctorNode.hpp"
#include "archive/FreeFunctionNode.hpp"
#include "utility/Timer.hpp"

#include "nodes/FlowNode.hpp"
#include "nodes/EventNodeV1.hpp"
#include "script/FlowScript.hpp"
#include "nodes/EventNode.hpp"
#include "nodes/ConversionNode.hpp"
#include "utility/dbgln.hpp"


namespace nf {


#define NF_SERIALIZE_INPUT(port)		 \
if constexpr (port.streamable)			 \
{										 \
	if (auto input = getInputData(port)) \
	{									 \
		Serializer << *input;			 \
		return true;					 \
	}									 \
	return false;						 \
}									     \
return false;							 \


	class IntegerAddNode : public nf::FlowNode
	{
		NF_NODE_NAME("IntegerAddNode")

	public:
		Expected<void, Error> setup() override
		{
			addPort(inputPort1, "MyInputPort1");
			addPort(resultPort, "MyResultPort");
			addPort(inputPort2, "MyInputPort2");

			return {};
		}

		void process() override
		{
			auto input1 = getInputData(inputPort1);
			auto input2 = getInputData(inputPort2);
			pprint(input1, input2);
			if (input1 && input2)
				setOutputData(resultPort, *input1 + *input2);

			pprint(resultPort.value);
		}
		// 




	private:
		nf::InputPort<int> inputPort1;
		nf::InputPort<int> inputPort2;
		nf::OutputPort<int> resultPort;
	};

	class DoubleIntegerSourceNode : public nf::FlowNode
	{
		NF_NODE_NAME("DoubleIntegerSourceNode")

	public:
		Expected<void, Error> setup() override
		{
			addPort(sourcePort1, "MySourcePort1");
			addPort(sourcePort2, "MySourcePort2");

			return {};
		}

		void process() override
		{
		}


	private:
		nf::OutputPort<int> sourcePort1{ 5 };
		nf::OutputPort<int> sourcePort2{ 7 };
	};


#include "core/UUID.hpp"

	nf::UUID getUUID()
	{
		static auto uuid = nf::UUID::create();
		return uuid;
	}

	class TestClass
	{
	public:
		int memberMethod()
		{

			return 123;
		}

		static int staticMethod()
		{
			return 0;
		}


	};

	template<auto method, class clazz>
	class TestFunctor
	{



	public:


		TestFunctor(const TestClass& clazz)
			: c(clazz)
		{

		}

		void process()
		{
			auto e = std::invoke(method, c);
			pprint(e);

		}
		clazz c;
	};
#include <cmath>

	double addNumbers(int a, int b)
	{
		return (a + b);
	}

	template<auto method>
	class ClassMethodNode : public FlowNode
	{
	public:
		using MSig_t = nf::FuncSignature<std::function<decltype(method)>>;
		using MClass_t = MSig_t::ClassType_t;
		using MReturn_T = MSig_t::ReturnType_t;
		using MArgument_ts = MSig_t::ParamTypes_t;


	public:
		InputPort<MClass_t> m_thisPort;
	};



	class CustomEvent : public FlowEvent
	{
		NF_REGISTER_EVENT(CustomEvent);
	public:
		CustomEvent(int a, int b)
			: m_a(a), m_b(b)
		{}



	public:
		int m_a;
		int m_b;


	};


	class ScriptEvent
	{
	public:





	private:
		std::unique_ptr<FlowEvent> event;
	};



	template<typename Event, class... Args>
	void myEvent(Args&&... args)
	{
		using types = std::tuple<nf::deduce_member_type_t<Args>...>;
	}


	struct MyEvent
	{
		int keyCode;
		bool successfull;
	};




	template<class U, class B>
	struct AsTuple
	{
		using type = std::pair<U, B>;
	};



	template<typename Event, typename Key, typename... Args>
	void importEvent(std::pair<Key, Args>&& ...args) {
		using fieldTypes = std::tuple<nf::deduce_member_type_t<Args>...>;
		std::vector<std::string_view> vec{ {args.first}... };
		// 	pprint(type_name<decltype(args.first)>()...);
	}


	template<auto Callable>
	std::unique_ptr<FlowNode> importConversion(const std::string& namePath)
	{
		using FSig_t = FuncSignature<decltype(std::function{ Callable }) > ;
		using To_t = FSig_t::ReturnType_t;
		using From_ts = FSig_t::ParamTypes_t;
		static_assert(std::tuple_size_v<From_ts> == 1, "Callable must be of signature: 'ToType Callable(FromType)'");
		using From_t = std::tuple_element_t<0, From_ts>;

		return std::make_unique< ConversionNodeImpl<From_t, To_t, Callable>>();
	}

	std::string converter(int from)
	{
		return std::to_string(from);
	}

	double addNumbers2(int a, int b)
	{
		return a + b;
	}

	int strToInt(std::string str)
	{
		return std::stoi(str);
	}



}




struct Position
{
	float x;
	float y;
};

struct Children
{
	std::vector<int> childs;
};

struct IsMovable
{
	bool movable = false;
};


auto allEntities(const entt::registry& reg)
{
	std::vector<entt::entity> entities;
	reg.each([&](auto entity) {
		entities.push_back(entity);
	});
	return entities;
}

namespace nf
{
// 	class Scene;

// 	class Entity;

// 	class Entity
// 	{
// 	public:
// 		Entity(entt::entity handle)
// 			: m_handle(handle)
// 		{}
// 		
// // 		template<typename Component, typename... Args>
// // 		Component& addComponent(Args&&... args)
// // 		{
// // 			return m_scene.m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
// // 		}
// 
// // 		template<typename Component>
// // 		Component& getComponent()
// // 		{
// // 			return m_scene.m_registry.get
// // 		}
// // 
// 
// 	private:
// 		entt::entity m_handle;
// // 	};
// // 
// 	class Scene
// 	{
// 	public:
// 		Scene() = default;
// 
// 		Entity createEntity()
// 		{
// 			auto enttentity = m_registry.create();
// 			Entity entity{ enttentity, *this };
// 			return entity;
// 		}
// 
// // 		template<typename... Components, typename...Args>
// // 		Entity createEntity(Args&&... args)
// // 		{
// // 			auto entity = createEntity();
// // 			entity.addComponent()
// // 
// // 		}
// 
// 	private:
// 		entt::registry m_registry;
// 		std::vector<Entity> m_entities;
// 
// 
// 		friend class Entity;
// 	};
}
#include "utility/ECS.hpp"




template<auto Func>
struct Reflector
{
	static constexpr auto FunctionName() noexcept
	{
		std::string_view pretty_function{ ENTT_PRETTY_FUNCTION };
		auto first = pretty_function.find_first_not_of(' ', pretty_function.find_first_of(ENTT_PRETTY_FUNCTION_PREFIX) + 1);
		auto value = pretty_function.substr(first, pretty_function.find_last_of(ENTT_PRETTY_FUNCTION_SUFFIX) - first);
		auto valuebefore = value.substr(0, value.find_first_of('('));
		auto valuelast = valuebefore.substr(valuebefore.find_last_of(' ') + 1);
		return valuelast;
	}
};

namespace nf
{
	template<typename T>
	struct B
	{
		class UK2Node_FunctionEntry;
		class UFunction;
		template<typename C>
		static const UK2Node_FunctionEntry* FindLocalEntryPoint(const UFunction* Function) {
			return nullptr;
		}

	};
}
using namespace nf;


#include "3rdparty/nameof/include/nameof.hpp"

using AddFunction = int (*)(int, int);

template<typename Ret, class... Args>
Ret callPluginFunction(const std::string& pluginDLLName, const std::string& functionName, Args&&... args)
{
	/**
	How to compile as dll
	1. C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2022\Visual Studio Tools\VC 
	2. Start -> x64 Native Tools Command Prompt for VS 2022
	3. CD to Folder plugin.cpp file lies 
	4. cl.exe /D_USRDLL /D_WINDLL /std:c++20 /EHsc mydll.cpp /link /DLL /OUT:plugin.dll 
	5. Ensure same Flags as main Program.
	*/
	using PluginFunction_t = Ret(*)(Args...);
	auto pluginDLL = LoadLibraryA(pluginDLLName.c_str());
	if (pluginDLL)
	{
		PluginFunction_t pluginFunction = reinterpret_cast<PluginFunction_t>(GetProcAddress(pluginDLL, functionName.c_str()));
		Ret ret = pluginFunction(args...);
		FreeLibrary(pluginDLL);
		return ret;
	}
	dbgln("Failed to load dll {}", "plugin.dll");
	return {};
}

template<typename Ret, class... Args>
struct Dummy
{
	using type = Ret(*)(Args...);
};


struct DummyEvent {
	int a = 30;
	int b = 40;
};



auto create_example_json()
{
	nlohmann::json j;

	// add a number that is stored as double (note the implicit conversion of j to an object)
	j["pi"] = 3.141;

	j["double_type_id"] = nf::type_id<double>();

	j["UUID"] = static_cast<std::uint64_t>(nf::UUID::create());

	// add a Boolean that is stored as bool
	j["happy"] = true;

	// add a string that is stored as std::string
	j["name"] = "Niels";

	// add another null object by passing nullptr
	j["nothing"] = nullptr;

	// add an object inside the object
	j["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)

	// add another object (using an initializer list of pairs)
	j["object"] = { {"currency", "USD"}, {"value", 42.99} };

	j["list"] = { j["object"], j["object"], j["object"] };


	return j.dump(4);

}



int main()
{ 

// 	size_t fmVersionHash = 30;
// 	std::ifstream scriptFile("dummy.fs");
// 	try {
// 		auto j = nlohmann::json::parse(scriptFile);
// 
// 		fmVersionHash = j["meta"]["flowModuleVersionHash"].get<size_t>();
// 		dbgln("Version Hash is: {}", fmVersionHash);
// 	}
// 	catch (std::exception& e)
// 	{
// 		dbgln(e.what());
// 
// 	}
// 	auto destination = pfd::save_file("Save Script", ".",
// 		{"Image Files (*.fscript)", "*.fscript"},
// 		pfd::opt::force_overwrite).result();
// 	dbgln(destination);
// 
// 	std::ofstream jsonfile(destination);
// 	jsonfile << fmVersionHash;



	
// 	auto firstNode = j["nodes"].get<std::vector<std::string>>();
// 	dbgln(firstNode);

	
	






// 	dbgln(j.dump(4));









// 
// 	dbgln(j["list"].get<std::vector<int>>());
// 	constexpr auto b = nf::type_id<double>();
// 	dbgln(b);

	exit(0);

// 	dbgln(callPluginFunction<int, int, int>("nodeflow/plugin.dll", "Multiply", 2, 3));

	auto k = nf::GeneratedEventNode<DummyEvent, &DummyEvent::a>();
// 	print(k.)
	
	k.constructFromEvent(DummyEvent{ 300,400 });





// 	dbgln(em.getComponents<OutputPort<int >>(SummationNode).value);









	return 0;


// 	auto nodeModule = std::make_shared<FlowModule>("FirstModule");
// 	nodeModule->registerCustomNode<IntegerAddNode>("Basics/IntegerAddNode").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerCustomNode<DoubleIntegerSourceNode>("Basics/DoubleIntegerSourceNode").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<int>("Types/Integer").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<std::string>("Types/String").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerConversion<strToInt>("Converters/StringToInteger").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<double>("awad").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<char>("awd/awgawd").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<bool>("awwagd").or_else([](auto Error) {pprint(Error); });
// 	nodeModule->registerType<long>("aga/awdag").or_else([](auto Error) {pprint(Error); });
// 
// 	return 0;





// // 
// 	nodeModule->registerType<FlowModule>("Types/Integere").or_else([](auto Error) {pprint(Error); });
// // 	// 
// // 
// // 
// // 
// // 
// 	FlowScript script(nodeModule);
// // // 
// 	auto str = script.spawnNode("Types/String").value();
// 	auto str2int = script.spawnNode("Converters/StringToInteger").value();
// // 
// 	script.setNodeOutputFromStr(str, 0, "3.1414141");
// 	script.setNodeOutputFromStr(str2int, 0, "3.1414141");
// // // 
// 	auto& atlas = TypenameAtlas::instance();
// 
// 	dbgln(script.findNode(str2int)->nodeName());
// 	dbgln(atlas.listTypenames());
// 	dbgln(atlas.listTypenames());
	
	
// 
// 	pprint(doubleIntegerSourceNode_.getArchetype());
// 	script.connectNodes(doubleIntegerSourceNode_, 0, addNumbersNode, 0).or_else([](auto Error) {pprint(Error); });
// 	script.connectNodes(doubleIntegerSourceNode_, 1, addNumbersNode, 1).or_else([](auto Error) {pprint(Error); });

// 	script.connectNodes(doubleIntegerSourceNode_, 0, integerToStringNode, 0).or_else([](auto Error) {pprint(Error); });


	return 0;

// 	script.connectNodes(doubleIntegerSourceNode_, 0, addNumbersNode, 0).or_else([](auto Error) {pprint(Error); });
// 	script.connectNodes(doubleIntegerSourceNode_, 1, addNumbersNode, 1).or_else([](auto Error) {pprint(Error); });;


	




// 	addNumbersNode.process();
// 	script.connectNodes(doubleIntegerSourceNode_.uuid(), 0, integerAddNode_.uuid(), 0).or_else([](auto Error) {pprint(Error); });
// 	script.connectNodes(doubleIntegerSourceNode_.uuid(), 1, integerAddNode_.uuid(), 1).or_else([](auto Error) {pprint(Error); });

	return 0;

// 	script.disconnectNode(doubleIntegerSourceNode_, 0, integerAddNode_, 0);
// 	script.disconnectNode(doubleIntegerSourceNode_, 1, integerAddNode_, 1);












// 	integerAddNode_.makeConnection(ConnectionPolicy::InputToOutput, 0, doubleIntegerSourceNode_, 0);
// 	integerAddNode_.makeConnection(ConnectionPolicy::InputToOutput, 1, doubleIntegerSourceNode_, 1);


// 	integerAddNode_.process();




// 
// 
// // 	static_assert(std::is_base_of_v<nf::Node, FlowModule> ==false, "<Type> not allowed to be of type <nf::Node>. Use 'importNode(...)' instead");
// 
// // 	auto& createFunctor = nodeModule.m_nodeCreators["Basics/TestNode"];
// // 
// // 	auto b = createFunctor();
// // 	pprint(b->getArchetype());
// 
// 
// 
// 	return 0;
// 	std::unique_ptr<nf::Node> integerAddNode = std::make_unique<IntegerAddNode>();
// 	std::unique_ptr<nf::Node> doubleIntegerSourceNode = std::make_unique<DoubleIntegerSourceNode>();
// 	std::unique_ptr<nf::FlowNode> flowNodeExample = std::make_unique<FlowNode>();
// 
// 	integerAddNode->setup();
// 	doubleIntegerSourceNode->setup();
// 
// 
// 	return 0;
// 
// 
// 
// 	flowNodeExample->setFlowNext(*integerAddNode);
// 	flowNodeExample->setFlowBefore(*doubleIntegerSourceNode);
// 
// 
// 	pprint(flowNodeExample->getFlowBefore(), &(*doubleIntegerSourceNode));
// 
// 	pprint(flowNodeExample->getFlowNext(), &(*doubleIntegerSourceNode));
// 
// 
// 	return 0;
// 	auto success1 = doubleIntegerSourceNode->makeConnection(nf::ConnectionPolicy::OutputToInput, 0, *integerAddNode, 0);
// // // 
// 	auto success2 = doubleIntegerSourceNode->makeConnection(nf::ConnectionPolicy::OutputToInput, 1, *integerAddNode, 1);
// 
// 
// 
// 
// 
// 	if (!success1)
// 		pprint(success1.error());
// 
// 	if (!success2)
// 		pprint(success2.error());
// 
// 
// // 
// // 	pprint(doubleIntegerSourceNode->getOutputPort(0).links());
// // 	pprint(doubleIntegerSourceNode->getOutputPort(1).links());
// // 	pprint(&(*integerAddNode));
// 
// // 	pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput, 0, *integerAddNode, 0));
// // 	pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput, 1, *integerAddNode, 1));
// 
// 
// // 	pprint(integerAddNode->getInputPort(0).link());
// // 	pprint(integerAddNode->getInputPort(1).link());
// // 	pprint(&(*doubleIntegerSourceNode));
// 
// 
// // 	pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput, 0, *integerAddNode, 0));
// 
// // 	pprint(integerAddNode->breakConnection(nf::ConnectionPolicy::InputToOutput, 0, *doubleIntegerSourceNode, 0));
// // 	pprint(integerAddNode->breakConnection(nf::ConnectionPolicy::InputToOutput, 1, *doubleIntegerSourceNode, 1));
// // 
// // 	doubleIntegerSourceNode->breakAllConnections(PortDirection::Both);
// 
// 
// 
// 
// 	integerAddNode->process();
// 
// 
// 
// 	
// 
// 
// 
// 
// 
// // 	std::ostringstream s;
// // 	integerAddNode->formatLinkageTree(s);
// // 	pprint(s.str());
// 
// 	std::ostringstream s2;
// 	doubleIntegerSourceNode->formatLinkageTree(s2);
// 	pprint(s2.str());
// 
// 	// 	pprint(integerAddNode->nodeName());
// 	// 	pprint(integerAddNode->portCount(nf::PortDirection::Output));
// 
// 
// 
// 
// 
// 
	return 0;

}

