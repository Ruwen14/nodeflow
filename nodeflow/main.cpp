// #include "core/NFAbstractNodeModel.hpp"
#pragma once
#include "nodeflow/core/FlowModule.hpp"
#include "nodeflow/core/FlowScript.hpp"
#include "nodeflow/nodes/FlowNode.hpp"
#include "nodeflow/utility/dbgln.hpp"

#include <algorithm>
#include <string>
#include <type_traits>
#include <unordered_set>

// #ifdef _WIN32
// #pragma comment(lib, "liblua54.a")
// #endif

// namespace nf
// {
// 	struct Serializer
// 	{
// 		uint64_t id;
// 		std::function<void(std::ostringstream&,
// void*)> func;
// 	};
//
// 	template<typename T>
// 	Serializer MakeSerializer()
// 	{
// 		auto lambda = [](std::ostringstream&
// stream, void* data)
// { 			stream <<
// *static_cast<T*>(data);
// 		};
//
// 		return { entt::type_hash<T>::value(),
// lambda };
// 	}
// }

// using Serializer =
// std::function<void(std::stringstream& archive,
// void* data)>;
//
// template<typename T>
// Serializer GenerateSerializer()
// {
// 	static_assert(nf::has_ostream_operator_v<T>,
// "No << operator found."); 	auto lambda =
// [](std::stringstream& archive, void* data) {
// archive << *static_cast<T*>(data);
// 	};
//
// 	return lambda;
// }
//
// struct CPPCodeInstruction
// {
// 	enum InstructionType
// 	{
// 		VariableInitialization,
// 		VariableSet,
// 		VariableGet,
// 		FreeFunctionCall,
// 		MemberFunctionCall
// 	};
//
// 	template<auto func, bool e = false>
// 	static CPPCodeInstruction
// GenCallInstruction_FreeFunction()
// 	{
// 		using codeContext =
// nf::FunctionCodeContext<func>; 		using
// retType = codeContext::ReturnType;
//
// 		CPPCodeInstruction ci;
// 		ci.instructionType =
// CPPCodeInstruction::FreeFunctionCall;
//
// 		std::stringstream stream;
// 		if constexpr
// (!std::is_same_v<retType, void>)
// 		{
// 			stream <<
// codeContext::ReturnTypeName() << " $r1 =
// ";
// 		}
//
// 		stream << codeContext::FunctionName()
// << "(";
//
// 		if constexpr (constexpr int argCount
// = codeContext::ArgumentCount()
// != 0)
// 		{
// 			ci.functionArgumentCount =
// argCount; 			auto argNames =
// codeContext::ArgumentTypeNames();
//
// 			for (size_t i = 0; i <
// argNames.size(); i++)
// 			{
// 				stream << "$a" << i +
// 1;
//
// 				if (i != argNames.size() -
// 1)
// 				{
// 					stream << ",
// ";
// 				}
// 			}
// 		}
//
// 		stream << ");";
//
// 		ci.instruction = stream.str();
// 		return ci;
// 	}
//
// 	InstructionType instructionType;
// 	int functionArgumentCount = -1;
// 	std::string instruction;
// };

// int fib(int n)
// {
// 	if (n <= 1)
// 		return n;
// 	return fib(n - 1) + fib(n - 2);
// }
//
// namespace nf {
// #define NF_SERIALIZE_INPUT(port)		 \
// if constexpr (port.streamable)
// \
// { \
// 	if (auto input = getInputData(port)) \
// 	{ \
// 		Serializer << *input;
// \
// 		return true;
// \
// 	} \
// 	return false;
// \
// } \
// return false; \
//
// 	class IntegerAddNode : public nf::FlowNode
// 	{
// 	public:
// 		Expected<void, Error> setup()
// override
// 		{
// 			addPort(inputPort1,
// "MyInputPort1"); addPort(resultPort,
// "MyResultPort"); addPort(inputPort2,
// "MyInputPort2");
//
// 			return {};
// 		}
//
// 		void process() override
// 		{
// 			auto input1 =
// getInputData(inputPort1); 			auto
// input2 = getInputData(inputPort2); 			if
// (input1
// && input2); 			setOutputData(resultPort,
// std::sin<int>(fib(*input1
// + *input2)));
// 		}
//
// 	private:
// 		nf::InputPort<int> inputPort1;
// 		nf::InputPort<int> inputPort2;
// 		nf::OutputPort<double> resultPort;
// 	};
//
// 	class DoubleIntegerSourceNode : public
// nf::FlowNode
// 	{
// 	public:
// 		Expected<void, Error> setup()
// override
// 		{
// 			addPort(sourcePort1,
// "MySourcePort1"); addPort(sourcePort2,
// "MySourcePort2");
//
// 			return {};
// 		}
//
// 		void process() override
// 		{
// 		}
//
// 	private:
// 		nf::OutputPort<int> sourcePort1{ 4
// }; 		nf::OutputPort<int> sourcePort2{ 5 };
// 	};
//
// #include "core/UUID.hpp"
//
// 	nf::UUID getUUID()
// 	{
// 		static auto uuid =
// nf::UUID::create(); 		return uuid;
// 	}
//
// 	class TestClass
// 	{
// 	public:
// 		int memberMethod()
// 		{
// 			return 123;
// 		}
//
// 		static int staticMethod()
// 		{
// 			return 0;
// 		}
// 	};
//
// 	template<auto method, class clazz>
// 	class TestFunctor
// 	{
// 	public:
//
// 		TestFunctor(const TestClass&
// clazz) 			: c(clazz)
// 		{
// 		}
//
// 		void process()
// 		{
// 			auto e = std::invoke(method,
// c); 			pprint(e);
// 		}
// 		clazz c;
// 	};
// #include <cmath>
//
// 	double addNumbers(int a, int b)
// 	{
// 		return (a + b);
// 	}
//
// 	template<auto method>
// 	class ClassMethodNode : public FlowNode
// 	{
// 	public:
// 		using MSig_t =
// nf::FuncSignature<std::function<decltype(method)>>;
// using MClass_t = MSig_t::ClassType_t;
// using MReturn_T = MSig_t::ReturnType_t;
// using MArgument_ts = MSig_t::ParamTypes_t;
//
// 	public:
// 		InputPort<MClass_t> m_thisPort;
// 	};
//
// 	class CustomEvent : public FlowEvent
// 	{
// 		NF_REGISTER_EVENT(CustomEvent);
// 	public:
// 		CustomEvent(int a, int b)
// 			: m_a(a), m_b(b)
// 		{}
//
// 	public:
// 		int m_a;
// 		int m_b;
// 	};
//
// 	class ScriptEvent
// 	{
// 	public:
//
// 	private:
// 		std::unique_ptr<FlowEvent> event;
// 	};
//
// 	template<typename Event, class... Args>
// 	void myEvent(Args&&... args)
// 	{
// 		using types =
// std::tuple<nf::deduce_member_type_t<Args>...>;
// 	}
//
// 	struct MyEvent
// 	{
// 		int keyCode;
// 		bool successfull;
// 	};
//
// 	template<class U, class B>
// 	struct AsTuple
// 	{
// 		using type = std::pair<U, B>;
// 	};
//
// 	template<typename Event, typename Key,
// typename... Args> 	void
// importEvent(std::pair<Key, Args>&& ...args) {
// 		using fieldTypes =
// std::tuple<nf::deduce_member_type_t<Args>...>;
// std::vector<std::string_view> vec{
// {args.first}... };
// 		//
// pprint(type_name<decltype(args.first)>()...);
// 	}
//
// 	template<auto Callable>
// 	std::unique_ptr<FlowNode>
// importConversion(const std::string& namePath)
// 	{
// 		using FSig_t =
// FuncSignature<decltype(std::function{ Callable
// }) > ; 		using To_t =
// FSig_t::ReturnType_t; using From_ts =
// FSig_t::ParamTypes_t;
// 		static_assert(std::tuple_size_v<From_ts>
// == 1, "Callable must be of signature: 'ToType
// Callable(FromType)'"); using From_t =
// std::tuple_element_t<0, From_ts>;
//
// 		return std::make_unique<
// ConversionNodeImpl<From_t, To_t, Callable>>();
// 	}
//
// 	std::string converter(int from)
// 	{
// 		return std::to_string(from);
// 	}
//
// 	double addNumbers2(int a, int b)
// 	{
// 		return a + b;
// 	}
//
// 	int strToInt(std::string str)
// 	{
// 		return std::stoi(str);
// 	}
// }
//
// struct Position
// {
// 	float x;
// 	float y;
// };

// struct Children
// {
// 	std::vector<int> childs;
// };
//
// struct IsMovable
// {
// 	bool movable = false;
// };
//
// auto allEntities(const entt::registry& reg)
// {
// 	std::vector<entt::entity> entities;
// 	reg.each([&](auto entity) {
// 		entities.push_back(entity);
// 	});
// 	return entities;
// }

// namespace nf
// {
// 	// 	class Scene;
//
// 	// 	class Entity;
//
// 	// 	class Entity
// 	// 	{
// 	// 	public:
// 	// 		Entity(entt::entity
// handle)
// 	// 			: m_handle(handle)
// 	// 		{}
// 	//
// 	// // 		template<typename
// Component, typename... Args>
// 	// // 		Component&
// addComponent(Args&&... args)
// 	// // 		{
// 	// // 			return
// m_scene.m_registry.emplace<Component>(m_handle,
// std::forward<Args>(args)...);
// 	// // 		}
// 	//
// 	// // 		template<typename
// Component>
// 	// // 		Component& getComponent()
// 	// // 		{
// 	// // 			return
// m_scene.m_registry.get
// 	// // 		}
// 	// //
// 	//
// 	// 	private:
// 	// 		entt::entity m_handle;
// 	// // 	};
// 	// //
// 	// 	class Scene
// 	// 	{
// 	// 	public:
// 	// 		Scene() = default;
// 	//
// 	// 		Entity createEntity()
// 	// 		{
// 	// 			auto enttentity =
// m_registry.create();
// 	// 			Entity entity{
// enttentity, *this };
// 	// 			return entity;
// 	// 		}
// 	//
// 	// // 		template<typename...
// Components, typename...Args>
// 	// // 		Entity
// createEntity(Args&&... args)
// 	// // 		{
// 	// // 			auto entity =
// createEntity();
// 	// // entity.addComponent()
// 	// //
// 	// // 		}
// 	//
// 	// 	private:
// 	// 		entt::registry m_registry;
// 	// 		std::vector<Entity>
// m_entities;
// 	//
// 	//
// 	// 		friend class Entity;
// 	// 	};
// }
//
// template<auto Func>
// struct Reflector
// {
// 	static constexpr auto FunctionName()
// noexcept
// 	{
// 		std::string_view pretty_function{
// ENTT_PRETTY_FUNCTION
// }; 		auto first =
// pretty_function.find_first_not_of(' ',
// pretty_function.find_first_of(ENTT_PRETTY_FUNCTION_PREFIX)
// + 1); auto value =
// pretty_function.substr(first,
// pretty_function.find_last_of(ENTT_PRETTY_FUNCTION_SUFFIX)
// - first); auto valuebefore = value.substr(0,
// value.find_first_of('(')); auto valuelast =
// valuebefore.substr(valuebefore.find_last_of('
// ') + 1); return valuelast;
// 	}
// };
//
// namespace nf
// {
// 	template<typename T>
// 	struct B
// 	{
// 		class UK2Node_FunctionEntry;
// 		class UFunction;
// 		template<typename C>
// 		static const
// UK2Node_FunctionEntry*
// FindLocalEntryPoint(const
// UFunction* Function) { 			return
// nullptr;
// 		}
// 	};
// }
// using namespace nf;
//
// nf::Node* PrepareCallWithNodeBackend()
// {
// 	auto nodeModule =
// std::make_shared<nf::FlowModule>("FirstModule");
// 	nodeModule->registerCustomNode<nf::IntegerAddNode>("Basics/IntegerAddNode").or_else([](auto
// Error) {pprint(Error);
// });
// nodeModule->registerCustomNode<nf::DoubleIntegerSourceNode>("Basics/DoubleIntegerSourceNode").or_else([](auto
// Error) {pprint(Error); });
//
// 	nf::FlowScript script(nodeModule);
// 	auto doubleIntegerSourceNode =
// script.spawnNode("Basics/DoubleIntegerSourceNode").value();
// auto intgerAddNode =
// script.spawnNode("Basics/IntegerAddNode").value();
//
// 	script.connectPorts(doubleIntegerSourceNode,
// 0, intgerAddNode, 0).or_else([](auto Error)
// {pprint(Error); });
// 	script.connectPorts(doubleIntegerSourceNode,
// 1, intgerAddNode, 1).or_else([](auto Error)
// {pprint(Error); });
//
// 	auto node =
// script.findNode(intgerAddNode);
//
// 	return node;
//
// 	//
// ankerl::nanobench::Bench().run("CallWithNodeBackend",
// [&] {
// 	//
// static_cast<nf::IntegerAddNode*>(node)->process();
// 	//
// ankerl::nanobench::doNotOptimizeAway(node);
// 	// 		});
//
// 	//
// dbgln(*node->getOutputPort(0).dataHandle().get<int>());
// }

int doStuff(int a)
{
    return 18;
}

template <typename T>
std::set<T> findDuplicates(std::vector<T> vec) // no-ref, no-const
{
    std::set<nf::typeid_t> duplicates;
    std::sort(vec.begin(), vec.end());
    std::set<nf::typeid_t> distinct(vec.begin(), vec.end());
    std::set_difference(vec.begin(),
                        vec.end(),
                        distinct.begin(),
                        distinct.end(),
                        std::inserter(duplicates, duplicates.end()));
    return duplicates;
}

class GenericConverter
{
    using Converter = bool (*)(const void* from, void* to);

public:
    template <typename Conversion>
    GenericConverter()
        : converter([](const void* from, void* to) -> bool { return Converter(from, to); })
    {
    }
    ~GenericConverter() = default;

    bool convert()
    {
        return converter(nullptr, nullptr);
    }

private:
    Converter converter = nullptr;
};

bool hey(const void* from, int* to)
{
    return false;
}
//
//  static bool convert(const AbstractConverterFunction* _this, const void* in,
//  void* out)
// {
//     const From* f = static_cast<const From*>(in);
//     To* t = static_cast<To*>(out);
//     const ConverterFunctor* _typedThis = static_cast<const
//     ConverterFunctor*>(_this); *t = _typedThis->m_function(*f); return true;
// }

// template <typename From, typename To, typename Callable>
// Type_Conversion type_conversion(const Callable& t_function)
// {
//     auto func = [t_function](const Boxed_Value& t_bv) -> Boxed_Value {
//         // not even attempting to call boxed_cast so that we don't get caught
//         in some call recursion return chaiscript::Boxed_Value(
//             t_function(detail::Cast_Helper<const From&>::cast(t_bv,
//             nullptr)));
//     };
// namespace nf
// {
// template <typename From, typename To, typename Conversion>
// decltype(auto) type_conversion(Conversion&& conversion)
// {
//     auto wrappedConversion = [conversion =
//     std::forward<Conversion>(conversion)](const void* from,
//                                                                                  void* to) ->
//                                                                                  bool {
//         const From* f = static_cast<const From*>(from);
//
//         return conversion(nullptr, nullptr);
//     };
//     return wrappedConversion;
// }
// } // namespace nf

namespace nf
{
template <std::size_t N>
struct StringLiteral
{
    consteval StringLiteral(const char (&str)[N]) noexcept
    {
        for (int i = 0; i < N; i++)
        {
            value[i] = str[i];
        }
    }

    char value[N];
};
} // namespace nf

#include <string_view>
template <typename T, nf::StringLiteral pname>
struct InputPin
{
    static auto name()
    {
        return std::string_view{ pname.value };
    }

    nf::PortIndex index() const
    {
        return m_index;
    }

    T value{};
    nf::PortIndex m_index = -1;
};

template <typename... Ports>
decltype(auto) helperExpandPortNames(Ports&&... p)
{
    return std::array<std::string_view, sizeof...(Ports)>{ { p.name()... } };
}

template <typename... Ports>
decltype(auto) helperSerializePorts(nf::PortIndex index, Ports&&... p)
{
    std::optional<std::string> result = std::nullopt;
    (..., (result = (index == p.index() ? p.getValueAsString() : result)));
    return result;
}
template <typename... Ports>
decltype(auto) helperDeserializePorts(nf::PortIndex index, const std::string& val, Ports&&... p)
{
    bool success = false;
    (..., (success = (index == p.index() ? p.setValueFromString(val) : success)));
    return success;
}

template <typename... Ports>
decltype(auto) helperPortNames(nf::PortIndex index, Ports&&... p)
{
    std::string result{ "Invalid Port" };
    (..., (result = (index == p.index() ? p.name() : result)));
    return result;
}

// Make as static constexpr member of Node
#define NF_REGISTER_INPUTS(...)                                                             \
public:                                                                                     \
    std::string portNameDontUse(nf::PortDirection dir, nf::PortIndex index) const           \
    {                                                                                       \
        static auto inPortNames{ helperExpandPortNames(__VA_ARGS__) };                      \
        NF_ASSERT(!(index == -1 || !(index < inPortNames.size())), "Port does not exist."); \
        if (index == -1 || !(index < inPortNames.size()))                                   \
            return "";                                                                      \
        return std::string(inPortNames[index]);                                             \
    }                                                                                       \
                                                                                            \
    std::string inPortName(nf::PortIndex index) const                                       \
    {                                                                                       \
        return helperPortNames(index, __VA_ARGS__);                                         \
    }

// std::string serializeOutput(nf::PortIndex index)
// {
// }

class TestNode
{
public:
    NF_REGISTER_INPUTS(p0, p1, p2)

    TestNode()
    {
        p0.m_index = 0;
        p1.m_index = 1;
        p2.m_index = 2;
    }

private:
    InputPin<double, "Input 1"> p0;
    InputPin<double, "Input 2"> p1;
    InputPin<double, "Input 3"> p2;
};

int main()
{
    TestNode n;

    dbgln(n.inPortName(3));

    // InputPin<double, "Input 1"> p0;
    // InputPin<double, "Input 2"> p1;
    // InputPin<double, "Input 3"> p2;
    //
    // p0.index = 0;
    // p1.index = 1;
    // p2.index = 2;

    nf::OutputPort<int> p0{ 30 };
    nf::OutputPort<double> p1{ 10.4 };
    nf::OutputPort<bool> p2{ true };

    p0.m_portIndex = 0;
    p1.m_portIndex = 1;
    p2.m_portIndex = 2;

    double k = 3.14151512313;
    std::cout << k;

    dbgln(helperDeserializePorts(1, "3.14151512313", p0, p1, p2));
    dbgln(helperSerializePorts(1, p0, p1, p2));

    //     TestNode n;
    //     dbgln(n.portName(nf::PortDirection::Input, 3));
    //     //     NF_INFO(n.portName(nf::PortDirection::Input, 1));

    //     auto k = expand_port_names(InputPin<double, "Input"> p0,
    //                                InputPin<double, "Input 1"> p1,
    //                                InputPin<double, "Input 2"> p2);

    //     auto k = NF_INPUT(p0, p1, p2);

    //     using c = decltype(d.inputs);
    //     NF_INFO(nf::type_name<c>());

    //     SPDLOG_INFO("hi");
    //     SPDLOG_ERROR("ERRO");
    //     spdlog::set_level(spdlog::level::trace);
    //     //  [2023-09-15 22:20:16.235] [info] [main.cpp:609] hi
    //     spdlog::set_pattern("[%T] [%^%l%$] [%s:%#]: %v");
    //
    //     //     [source %s] [function %!] [line %#] %v"
    //     SPDLOG_ERROR("ERRwwddwO");
    //     SPDLOG_TRACE("hiwSAdwddwdw");

    //     -> bool { return
    //     hey(from, to); };

    //     nf::dev::GenericConverter converter(hey);
    //     dbgln(converter.convert());

    //     std::set<nf::typeid_t> duplicates = findDuplicates(types);

    //     const auto& instance = TypenameAtlas::instance();

    auto module = std::make_shared<nf::FlowModule>("wdadwdwwwwdwdwdadwadwds");
    module->registerFunction<doStuff>("Functions/doStwuff");

    // #ToDo: set typeID virtual support;

    nf::FlowScript script(module);
    auto maybeNode =
        script.spawnNode("Functions/doStwuff").or_else([](auto Error) { NF_ASSERT(false, ""); });

    auto doStuffNode = script.findNode(*maybeNode);
    //     NF_NODE_LOG(doStuffNode->uuid(), "ewdrrowdwdr");

    script.spawnVariable("Hello", 140);

    //     if (doStuffNode)
    //     {
    //         dbgln(doStuffNode->getArchetype());
    //     }

    // 	doStuffNode->setOutputDataFromString();

    // 	dbgln(doStuffNode->setOutputDataFromStr
    // ing(0, "Hallo"));

    // 	script.spawn(nf::lang::while)
    // script.defineFlow(whileNode,
    // ifNode).defineFlow()
    // 	script.defineFlow(ifelseNode.flowPort("if"),
    // whileNode.inFlowPort())

    // 	script.connectPorts()
    // 	script.connectFlow(script.startNode(),
    // ifelseNode).
    //		   connectFlow(ifelseNode,
    // 	script.connectFlow(IfElseNode, "If",
    // ifelseNode) 	script.connectFlow(IfElseNode,
    // "Else", ifelseNode)
    //	script.setupFlowFrom(nf::FlowSequence {
    //			script.onScriptStart(),
    //
    //
    // })
    //
    return 0;

    // 	auto nodeModule =
    // std::make_shared<nf::FlowModule>("FirstModule");
    // 	nodeModule->registerCustomNode<nf::IntegerAddNode>("Basics/IntegerAddNode").or_else([](auto
    // Error) {pprint(Error); });
    // 	nodeModule->registerCustomNode<nf::DoubleIntegerSourceNode>("Basics/DoubleIntegerSourceNode").or_else([](auto
    // Error) {pprint(Error); });
    //
    // 	nf::FlowScript script(nodeModule);
    // 	auto doubleIntegerSourceNode =
    // script.spawnNode("Basics/DoubleIntegerSourceNode").value();
    // auto intgerAddNode =
    // script.spawnNode("Basics/IntegerAddNode").value();
    //
    // 	script.connectPorts(doubleIntegerSourceNode,
    // 0, intgerAddNode, 0).or_else([](auto Error)
    // {pprint(Error); });
    // 	script.connectPorts(doubleIntegerSourceNode,
    // 1, intgerAddNode, 1).or_else([](auto Error)
    // {pprint(Error); });

    // 	nodeModule->registerCustomNode<IntegerAddNode>("Basics/IntegerAddNode").or_else([](auto
    // Error) {pprint(Error);
    // });
    // nodeModule->registerCustomNode<DoubleIntegerSourceNode>("Basics/DoubleIntegerSourceNode").or_else([](auto
    // Error) {pprint(Error); });
    // nodeModule->registerType<int>("Types/Integer").or_else([](auto
    // Error) {pprint(Error);
    // });
    // nodeModule->registerType<std::string>("Types/String").or_else([](auto
    // Error) {pprint(Error); });
    // 	nodeModule->registerConversion<strToInt>("Converters/StringToInteger").or_else([](auto
    // Error) {pprint(Error);
    // });
    // nodeModule->registerType<double>("awad").or_else([](auto
    // Error) {pprint(Error); });
    // 	nodeModule->registerType<char>("awd/awgawd").or_else([](auto
    // Error) {pprint(Error); });
    // 	nodeModule->registerType<bool>("awwagd").or_else([](auto
    // Error) {pprint(Error); });
    // 	nodeModule->registerType<long>("aga/awdag").or_else([](auto
    // Error) {pprint(Error); });
    //
    // 	return 0;

    // //
    // 	nodeModule->registerType<FlowModule>("Types/Integere").or_else([](auto
    // Error) {pprint(Error); });
    // // 	//
    // //
    // //
    // //
    // //
    // 	FlowScript script(nodeModule);
    // // //
    // 	auto str =
    // script.spawnNode("Types/String").value();
    // 	auto str2int =
    // script.spawnNode("Converters/StringToInteger").value();
    // //
    // 	script.setNodeOutputFromStr(str, 0,
    // "3.1414141");
    // 	script.setNodeOutputFromStr(str2int, 0,
    // "3.1414141");
    // // //
    // 	auto& atlas = TypenameAtlas::instance();
    //
    // 	dbgln(script.findNode(str2int)->nodeName());
    // 	dbgln(atlas.listTypenames());
    // 	dbgln(atlas.listTypenames());

    //
    // 	pprint(doubleIntegerSourceNode_.getArchetype());
    // 	script.connectNodes(doubleIntegerSourceNode_,
    // 0, addNumbersNode, 0).or_else([](auto
    // Error) {pprint(Error); });
    // 	script.connectNodes(doubleIntegerSourceNode_,
    // 1, addNumbersNode, 1).or_else([](auto
    // Error) {pprint(Error); });

    // 	script.connectNodes(doubleIntegerSourceNode_,
    // 0, integerToStringNode, 0).or_else([](auto
    // Error) {pprint(Error);
    // });

    return 0;

    // 	script.connectNodes(doubleIntegerSourceNode_,
    // 0, addNumbersNode, 0).or_else([](auto
    // Error) {pprint(Error); });
    // 	script.connectNodes(doubleIntegerSourceNode_,
    // 1, addNumbersNode, 1).or_else([](auto
    // Error) {pprint(Error); });;

    // 	addNumbersNode.process();
    // 	script.connectNodes(doubleIntegerSourceNode_.uuid(),
    // 0, integerAddNode_.uuid(),
    // 0).or_else([](auto Error) {pprint(Error);
    // });
    // script.connectNodes(doubleIntegerSourceNode_.uuid(),
    // 1, integerAddNode_.uuid(),
    // 1).or_else([](auto Error) {pprint(Error);
    // });

    return 0;

    // 	script.disconnectNode(doubleIntegerSourceNode_,
    // 0, integerAddNode_, 0);
    // 	script.disconnectNode(doubleIntegerSourceNode_,
    // 1, integerAddNode_, 1);

    // 	integerAddNode_.makeConnection(ConnectionPolicy::InputToOutput,
    // 0, doubleIntegerSourceNode_, 0);
    // 	integerAddNode_.makeConnection(ConnectionPolicy::InputToOutput,
    // 1, doubleIntegerSourceNode_, 1);

    // 	integerAddNode_.process();

    //
    //
    // //
    // static_assert(std::is_base_of_v<nf::Node,
    // FlowModule>
    // ==false,
    // "<Type> not allowed to be of type
    // <nf::Node>. Use 'importNode(...)'
    // instead");
    //
    // // 	auto& createFunctor =
    // nodeModule.m_nodeCreators["Basics/TestNode"];
    // //
    // // 	auto b = createFunctor();
    // // 	pprint(b->getArchetype());
    //
    //
    //
    // 	return 0;
    // 	std::unique_ptr<nf::Node> integerAddNode =
    // std::make_unique<IntegerAddNode>();
    // std::unique_ptr<nf::Node>
    // doubleIntegerSourceNode =
    // std::make_unique<DoubleIntegerSourceNode>();
    // 	std::unique_ptr<nf::FlowNode>
    // flowNodeExample =
    // std::make_unique<FlowNode>();
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
    // 	pprint(flowNodeExample->getFlowBefore(),
    // &(*doubleIntegerSourceNode));
    //
    // 	pprint(flowNodeExample->getFlowNext(),
    // &(*doubleIntegerSourceNode));
    //
    //
    // 	return 0;
    // 	auto success1 =
    // doubleIntegerSourceNode->makeConnection(nf::ConnectionPolicy::OutputToInput,
    // 0, *integerAddNode, 0);
    // // //
    // 	auto success2 =
    // doubleIntegerSourceNode->makeConnection(nf::ConnectionPolicy::OutputToInput,
    // 1, *integerAddNode, 1);
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
    // //
    // pprint(doubleIntegerSourceNode->getOutputPort(0).links());
    // //
    // pprint(doubleIntegerSourceNode->getOutputPort(1).links());
    // // 	pprint(&(*integerAddNode));
    //
    // //
    // pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput,
    // 0, *integerAddNode, 0));
    // //
    // pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput,
    // 1, *integerAddNode, 1));
    //
    //
    // //
    // pprint(integerAddNode->getInputPort(0).link());
    // //
    // pprint(integerAddNode->getInputPort(1).link());
    // //
    // pprint(&(*doubleIntegerSourceNode));
    //
    //
    // //
    // pprint(doubleIntegerSourceNode->breakConnection(nf::ConnectionPolicy::OutputToInput,
    // 0, *integerAddNode, 0));
    //
    // //
    // pprint(integerAddNode->breakConnection(nf::ConnectionPolicy::InputToOutput,
    // 0, *doubleIntegerSourceNode, 0));
    // //
    // pprint(integerAddNode->breakConnection(nf::ConnectionPolicy::InputToOutput,
    // 1, *doubleIntegerSourceNode, 1));
    // //
    // //
    // doubleIntegerSourceNode->breakAllConnections(PortDirection::Both);
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
    // //
    // integerAddNode->formatLinkageTree(s);
    // // 	pprint(s.str());
    //
    // 	std::ostringstream s2;
    // 	doubleIntegerSourceNode->formatLinkageTree(s2);
    // 	pprint(s2.str());
    //
    // 	// pprint(integerAddNode->nodeName());
    // 	//
    // pprint(integerAddNode->portCount(nf::PortDirection::Output));
    //
    //
    //
    //
    //
    //
    return 0;
}