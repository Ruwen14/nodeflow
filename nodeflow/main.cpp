#include "core/NFAbstractNodeModel.hpp"

#include "core/type_tricks.hpp"

#include "../3rdparty/cpputils/prettyprint.h"





using namespace cpputils;

// #ifdef _WIN32
// #pragma comment(lib, "liblua54.a")
// #endif




#include "core/FreeFunctionNode.hpp"






#include "core/CodeGen.hpp"

#include "core/Reflection.hpp"

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



namespace nf
{
	class NodeDataModel;


	struct PortConnectionContext
	{
		NodeDataModel* linkedNode = nullptr;
		int portIndex = -1;
	};

	struct NodeOutputSocket
	{
		// opaque pointer on the value of the output-port
		PinDataHandle valueView;
		PortConnectionContext connectionContext;
	};

	class OutputBus
	{

	private:
		std::vector<NodeOutputSocket> sockets;
	};

	class NodeDataModel
	{
	public:
		NodeDataModel() = default;
		virtual ~NodeDataModel() = default;

		template<typename T>
		void setNodeOutput(const OutputPin<T>& port, const T& val);

		template<typename T>
		void setNodeOutput(int portIndex, const T& val);

		template<typename T>
		T* getNodeInput(const InputPin<T>& port);

	private:
	};
}



using Serializer = std::function<void(std::stringstream& archive, void* data)>;

template<typename T>
Serializer GenerateSerializer()
{
	static_assert(nf::has_ostream_operator_v<T>, "No << operator found.");
	auto lambda = [](std::stringstream& archive, void* data) {
		archive << *static_cast<T*>(data);
	};

	return lambda;
}


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
#include "core/FlowModule.hpp"
#include "core/NFNode.hpp"

#include "../3rdparty/cpputils/timer.h"

class CustomNode : public nf::NFNode
{
public:
	bool setup() override
	{

		addPort(inPor, "MyInPort");
		addPort(outPort, "MyOutPort");
		return true;
	}

	void process() override
	{
		const int c = 3;
		setOutputData(outPort, 3);
		

	}

private:
	nf::InputPort<int> inPor;
	nf::OutputPort<int> outPort;
};

#include "core/FlowEvent.hpp"

class CustomResizeEvent : public nf::FlowEvent
{
public:
	NF_REGISTER_EVENT(CustomResizeEvent)
	CustomResizeEvent(int x_, int y_,int width_, int height_) : 
		x(x_), y(y_), width(width_), height(height_)
	{}
	int x, y, width, height;
};


int main()
{	
	std::unique_ptr<nf::NFNode> node = std::make_unique<CustomNode>();
	node->setup();
	node->process();


	std::ostringstream s;
	node->serialize(s, nf::PortDirection::Output, 3);

	std::vector<int> a{ 3 };

	
	nf::detail::PortLink l;
	nf::OutputPort<int> k;
	pprint(node->nodeName());

// 	pprint(*node->outputPortList()[0].m_dataHandle.get<double>());
	return 0;

}

