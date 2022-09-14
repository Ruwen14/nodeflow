# nodeflow

Flexible C++ node-based Dataflow Framework, similiar to
https://github.com/cross-platform/dspatch and Unreal Engine's Blueprint system.

Still in _very_ early development.

## Planned Feature's
- [x] Qt Interfacing with QGraphicsScene https://doc.qt.io/qt-5/qgraphicsscene.html
- [x] Propagation of any data type without limitations
- [x] Dynamic Input and Output Pins
- [x] Easy Implementation of custom nodes 

## How to implement Execution
- Every Node stores a vector<Signal> output and vector<const Signal*> input, so the input of a node is just the view on the ouput of the connected node
- getInput -> returns const Template* that can be nullptr if not connected.
- When the input of a Node gets removed, the input will be set nullptr -> setInputDisconnected
# Execution Order
- Graph-Theory: Topological sort, Breadth/Depth First Search. Every Node needs a unique id tho (UUID)

# UUID
- See Chernos video for uin64_t uuid (enough)

## Look Into
- c++ version of retejs: https://github.dev/retejs/cpp-engine
- https://github.com/skypjack/entt/issues/658
- Look into https://skypjack.github.io/entt/md_docs_md_core.html
- Look into https://github.com/foonathan/string_id

FKismetCompilerContext::Compile https://docs.unrealengine.com/4.26/en-US/API/Editor/KismetCompiler/FKismetCompilerContext/Compile/
struct FFuncInfo

https://s1t2.com/blog/brief-intro-k2nodes
<!--
- Look into ChaiScript Types
  -  any.hpp https://github.com/ChaiScript/ChaiScript/blob/3aa1fa8278efaa369487f5a7203f3b483a6ae09c/include/chaiscript/dispatchkit/any.hpp
  -  boxed_value https://github.com/ChaiScript/ChaiScript/blob/3aa1fa8278efaa369487f5a7203f3b483a6ae09c/include/chaiscript/dispatchkit/boxed_value.hpp
  -  compile time typeid https://github.dev/ChaiScript/ChaiScript/blob/develop/include/chaiscript/dispatchkit/type_conversions.hpp -->
