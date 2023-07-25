import re
import os
import CppHeaderParser
from CppHeaderParser import CppMethod, CppVariable
# from cxxheaderparser.cxxheaderparser.simple import parse_string, parse_file
from typing import List, Dict, Tuple, Optional, Set
import itertools
import datetime

FUNCTION_SPECIFIER = "NF_FUNCTION"
FUNCTION_FORBIDDEN = ["template",
                      "typename"]


class ParseException(Exception):
    __module__ = Exception.__module__
    pass

class CodeGenException(Exception):
    __module__ = Exception.__module__
    pass


def range_subset(range1, range2):
    """Whether range1 is a subset of range2."""
    if not range1:
        return True  # empty range is subset of anything
    if not range2:
        return False  # non-empty range can't be subset of empty range
    if len(range1) > 1 and range1.step % range2.step:
        return False  # must have a single value or integer multiple step
    return range1.start in range2 and range1[-1] in range2


def clean_string(string: str):
    string = string.strip("'")
    string = string.strip('"')
    return string


def strtobool(boolean: str):
    boolean = boolean.lower()
    if boolean in ('y', 'yes', 't', 'true', 'on', '1'):
        return True
    elif boolean in ('n', 'no', 'f', 'false', 'off', '0'):
        return False
    return None


class CodeGenCache(object):
    def __init__(self, cache_dir: str = None, file_cache_extension: str = None):
        self._file_cache_extension = file_cache_extension
        self._cache_dir = cache_dir
        if not cache_dir:
            self._cache_dir = ".codegencache"

        if not file_cache_extension:
            self._file_cache_extension = "codegen.cache"

        os.makedirs(self._cache_dir, exist_ok=True)

    def store(self, module: str, generated: str):
        cache_file_name = os.path.join(self._cache_dir, module + '.' + self._file_cache_extension)
        with open(cache_file_name, 'w') as handle:
            handle.write(generated)

    def retrieve(self, module: str) -> Optional[str]:
        cache_file_name = os.path.join(self._cache_dir, module + '.' + self._file_cache_extension)

        if not os.path.isfile(cache_file_name):
            return None

        with open(cache_file_name, "r") as handle:
            string = handle.read()
        return string


class Namespace:
    def __init__(self, name: str, span: range):
        self.name = name
        self.span = span

    def __str__(self):
        return f"Namespace[name={self.name}, " \
               f"range={self.span.start}:{self.span.stop}]"

    def __repr__(self):
        return str(self)


class Annotation(object):
    def __init__(self, declaration: str, supported_annotation_attributes):
        self.SUPPORTED_ATTRIBUTES = supported_annotation_attributes
        self.declaration: str = declaration
        self.attributes: Dict = self._parse_attributes()

    def __str__(self):
        return self.declaration

    def __repr__(self):
        return str(self)

    def _find_attributes(self, string):
        open_parenthese = string.find('(') + 1
        close_parenthese = string.rfind(')')

        if open_parenthese == 0 or close_parenthese == -1:
            raise ParseException(f"Missing parenthese in '{string}'")

        attributes = string[open_parenthese:close_parenthese].strip()

        if attributes:
            # https://stackoverflow.com/questions/18893390/splitting-on-comma-outside-quotes
            pattern = r",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)"
            attributes = re.split(pattern, attributes)

            return attributes
        return None

    def _parse_attributes(self):
        attributes_dict = dict()
        annotation_attributes = self._find_attributes(self.declaration)

        if not annotation_attributes:
            return attributes_dict

        for attr in annotation_attributes:
            attr = attr.strip()
            if '=' in attr:
                res = attr.split("=")
                if len(res) != 2:
                    raise AttributeError(f"Unable to resolve tag '{attr}' in '{self.declaration}'")
                key, val = res
                key = clean_string(key.strip())
                val = clean_string(val.strip())
                attributes_dict[key] = val
            else:
                attributes_dict[attr] = None

        # check if attributes are supported
        for key in attributes_dict.keys():
            if key not in self.SUPPORTED_ATTRIBUTES:
                raise AttributeError(f"Unkown attribute '{key}' in '{self.declaration}' \n"
                                     f"\t\t\t\tSupported attributes are: '{list(self.SUPPORTED_ATTRIBUTES.keys())}'")

            expected_type = self.SUPPORTED_ATTRIBUTES[key]
            if expected_type == bool:
                val = strtobool(attributes_dict[key])
                if val is None:
                    raise AttributeError(
                        f"Unable to resolve '{attributes_dict[key]}' as boolean with '{key}'='{attributes_dict[key]}'"
                        f" in {self.declaration}")
                attributes_dict[key] = val

        return attributes_dict


class FunctionAnnotation(Annotation):
    SUPPORTED_ATTRIBUTES = {"DisplayName": str, # DisplayName should be OverrideName
                            "Category": str,
                            "ToolTip": str,
                            "Nativizable": None,
                            "ReturnDisplayName": str,
                            "NonConstRefArgsAsOut": bool
                            }

    def __init__(self, annotation: str):
        super().__init__(annotation, FunctionAnnotation.SUPPORTED_ATTRIBUTES)


class ClassAnnotation(Annotation):
    SUPPORTED_ATTRIBUTES = {"DisplayName": str,
                            "Category": str
                            }

    def __init__(self, annotation: str):
        super().__init__(annotation, ClassAnnotation.SUPPORTED_ATTRIBUTES)

class EventAnnotation(Annotation):
    SUPPORTED_ATTRIBUTES = {"OverrideName": str,
                            "Category": str
                            }

    def __init__(self, annotation: str):
        super().__init__(annotation, EventAnnotation.SUPPORTED_ATTRIBUTES)


class NFFunction(object):
    def __init__(self, annotation: str, declaration: str, span: range, namespace: Optional[str] = None):
        self._annotation: FunctionAnnotation = FunctionAnnotation(annotation)
        self._declartion: str = declaration
        self._span = span
        self._namespace = namespace
        self.func_info: CppMethod = self._parse_declaration()

    def _parse_declaration(self):
        for forbidden in FUNCTION_FORBIDDEN:
            if forbidden in self._declartion:
                raise AssertionError(
                    f"Unable to resolve argument '{forbidden}' in: '{self._declartion}'")

        result = CppHeaderParser.CppHeader(self._declartion, argType="string")
        if not len(result.functions) > 0:
            raise RuntimeError(f"Could not parse NFFunction '{self._declartion}'")
        func_info: CppMethod = result.functions[0]
        return func_info

    def name(self) -> str:
        return self.func_info["name"]

    def parameters(self) -> List[CppHeaderParser.CppVariable]:
        return self.func_info["parameters"]

    def return_type(self)-> str:
        return self.func_info["rtnType"]

    def declaration(self) -> str:
        return self._declartion

    def annotation(self) -> FunctionAnnotation:
        return self._annotation

    def span(self) -> range:
        return self._span

    def namespace(self) -> str:
        return self._namespace


class NFClass(object):
    def __init__(self, annotation: str, declaration: str, span: range, namespace: Optional[str] = None):
        self._annotation: ClassAnnotation = ClassAnnotation(annotation)
        self._declartion: str = declaration
        self._span = span
        self._namespace = namespace
        self._methods: List[NFFunction] = []

        self._name = self._parse_declaration()
        # self.func_info: CppMethod = self._parse_declaration()

    def _parse_declaration(self):
        # Parse class name
        class_keyword = self._declartion.find("class") + 5
        curly_brace = self._declartion.find("{")
        assert class_keyword != 4 and curly_brace != -1

        class_name = self._declartion[class_keyword:curly_brace].strip()

        assert class_keyword is not None

        return class_name

    def find_and_set_methods(self, funcs: List[NFFunction]):
        for func in funcs:
            if range_subset(func.span(), self._span):
                self._methods.append(func)

    def name(self) -> str:
        return self._name

    def declaration(self) -> str:
        return self._declartion

    def annotation(self) -> ClassAnnotation:
        return self._annotation

    def span(self) -> range:
        return self._span

    def namespace(self) -> str:
        return self._namespace

    def methods(self) -> List[NFFunction]:
        return self._methods

class NFEvent(object):
    def __init__(self, annotation: str, declaration: str, span: range, namespace: Optional[str] = None):
        self._annotation: EventAnnotation = EventAnnotation(annotation)
        self._declartion: str = declaration
        self._span = span
        self._namespace = namespace
        self._event_args: CppHeaderParser.CppVariable = []

        self._parse_declaration()

    def _parse_declaration(self):
        result: CppHeaderParser.CppHeader = CppHeaderParser.CppHeader(self._declartion, argType="string")

        # print(len(result.classes))
        if not len(result.classes) == 1:
            raise ParseException(f"Could not parse NFEvent '{self._declartion}'")

        struct_info: dict = result.classes[list(result.classes.keys())[0]]

        for member in struct_info["properties"]["public"]:
            self._event_args.append(member)


    def declaration(self) -> str:
        return self._declartion

    def annotation(self) -> EventAnnotation:
        return self._annotation

    def span(self) -> range:
        return self._span

    def namespace(self) -> str:
        return self._namespace

    def event_args(self, pretty_name: bool = False) -> List[CppHeaderParser.CppVariable]:
        return self._event_args

    def event_arg_names(self) -> List[str]:
        return [arg['name'] for arg in self.event_args()]


class NodeBuilder(object):
    def __init__(self):
        self.code: str = ""

    def begin_node(self, class_name: str, parent: str):
        self.code += f'''class {class_name} : public {parent}  \n{{\n'''

    def end_node(self):
        self.code += "\n};"

    def add_scope(self, scope: str):
        self.code += f"{scope}:\n"

    def set_node_name(self, node_name: str):
        self.code += f"\tNF_NODE_NAME('{node_name}')\n"

    def add_input_port(self, dtype: str, var_name: str):
        if not var_name:
            raise CodeGenException(f"Empty parameter name not allowed.")
        self.code += f"\tnf::InputPort<{dtype}> ip_{var_name};\n"

    def add_output_port(self, dtype: str, var_name: str, default_value: str = None):
        if not var_name:
            raise CodeGenException(f"Empty parameter name not allowed.")
        if default_value:
            self.code += f"\tnf::OutputPort<{dtype}> op_{var_name}{{{default_value}}};\n"
        else:
            self.code += f"\tnf::OutputPort<{dtype}> op_{var_name};\n"

class FunctionNodeBuilder(NodeBuilder):
    def __init__(self, nf_function: NFFunction):
        super().__init__()
        self.func = nf_function

    def __call__(self):
        self.begin_node(f"NODE_{self.func.name()}", "nf::FlowNode")
        self.set_node_name("Replace Me")
        for idx, param in enumerate(self.func.parameters()):
            if idx == 0:
                self.add_scope("private")
            self.add_input_port(param["type"], f"{param['name']}")
        self.add_scope("private")
        self.add_output_port(self.func.return_type(), "returnValue")
        self.end_node()







class HeaderParser(object):
    PATTERN_REMOVE_COMMENTS = re.compile(r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"', re.DOTALL
                                         | re.MULTILINE)
    # Tokens include "{", "}" and "namespace *".
    PATTERN_NAMESPACE_TOKENS = re.compile(r"(\{|}|namespace\s+\w+(?:::\w+)*)")
    PATTERN_NF_FUNCTION = re.compile(r'(NF_FUNCTION(?:\(.*?\))?;?)\n(.*?[\{;])', re.MULTILINE | re.DOTALL)
    PATTERN_NF_CLASS = re.compile(r'(NF_CLASS(?:\(.*?\))?;?)\n(.*?(?:\}\s*;))', re.MULTILINE | re.DOTALL)
    PATTERN_NF_EVENT = re.compile(r'(NF_EVENT(?:\(.*?\))?;?)\n(.*?(?:\}\s*;))', re.MULTILINE | re.DOTALL)


    def __init__(self, header_file: str):
        self.filename: str = header_file
        self.modification_date: float = os.path.getmtime(header_file)

        file_content: str = HeaderParser.read_file_into_buffer(header_file)
        self.file_content: str = HeaderParser.remove_comments(file_content)

        self._namespace_tokens: List[Tuple[str, int]] = self._generate_namespace_tokens(self.file_content)
        self.namespaces = self._encode_namespaces(self._namespace_tokens)

        self.nf_functions: List[NFFunction] = self._find_functions_with_annotations(self.file_content)
        # self.nf_classes: List[NFClass] = self._find_classes_with_annotations(self.file_content)
        self._find_events_with_annotations(self.file_content)

        self.unique_types: List[str] = self._unique_types()

    @staticmethod
    def read_file_into_buffer(header_file: str) -> str:
        with open(header_file) as file:
            buffer = file.read()
        return buffer

    @staticmethod
    def remove_comments(text: str) -> str:
        def replacer(match):
            s = match.group(0)
            if s.startswith('/'):
                return " "  # note: a space and not an empty string
            else:
                return s

        return re.sub(HeaderParser.PATTERN_REMOVE_COMMENTS, replacer, text)

    def _unique_types(self) -> List[str]:
        unique_encountered_types = set()
        for func in self.nf_functions:
            types = func.func_info["parameters"]
            for typ in types:
                raw_type = typ["raw_type"]
                if raw_type:
                    unique_encountered_types.add(raw_type)

        return sorted(unique_encountered_types)

    def _generate_namespace_tokens(self, text: str) -> List[Tuple[str, int]]:
        """Generates tokens, that are used to find namespaces in header file.
            Tokens include "{", "}" and "namespace *" and their location in the file
        """
        ns_token_iter = HeaderParser.PATTERN_NAMESPACE_TOKENS.finditer(text)
        ns_tokens = [(ns_token.group(), ns_token.start()) for ns_token in ns_token_iter]

        return ns_tokens

    def _encode_namespaces(self, namespace_tokens: List[Tuple[str, int]]) -> List[Namespace]:
        """Finds namespaces in the header file and encodes their location.
        :return: List of Namespace
        """

        def _find_namespace_range(tokens: List[Tuple[str, int]]):
            openBr = 0
            itok = 1
            namespace_name, start_loc = tokens[itok]
            end_location = None
            while itok < len(tokens):
                tok, loc = tokens[itok]
                if tok == '{':
                    openBr += 1
                elif tok == '}':
                    openBr -= 1
                itok += 1
                if openBr == 0:
                    end_location = loc
                    break
            if end_location is None:
                raise ParseException(
                    f"Unable to resolve namespaces. Header file contains uneven ratio of enclosing '{{' and '}}' "
                    f"with {self.file_content.count('{')} / {self.file_content.count('}')}. \n                "
                    f"Check for presence of '{{' and '}}' in macros, comments or strings and remove them if possible."
                )
            return range(start_loc, end_location)

        namespaces = []
        for idx, token in enumerate(namespace_tokens):
            if "namespace" in token[0]:
                namespace = Namespace(name=token[0].split()[1], span=_find_namespace_range(namespace_tokens[idx:]))
                namespaces.append(namespace)

        # Build hierarchy of nested namespaces
        namespaces = sorted(namespaces, key=lambda r: r.span.stop)
        for namespace, namespace_other in itertools.combinations(namespaces, 2):
            if range_subset(namespace.span, namespace_other.span):
                namespace.name = namespace_other.name + "::" + namespace.name
                continue
            if range_subset(namespace_other.span, namespace.span):
                namespace_other.name = namespace.name + "::" + namespace_other.name
                continue

        return namespaces

    def _find_functions_with_annotations(self, text: str) -> List[NFFunction]:
        """Looks through the header files and extracts functions with the annotation 'NF_FUNCTION(...)'.
           For each function it encounters it constructs a NFFunction object, which includes the annotation data aswell
           the parsed function and alongside its data
        :return: List of NFFunction objects
        """

        print("--------------------------------------------------------------- Parsing NFFunctions "
              "---------------------------------------------------------------")

        nf_functions = []

        matches = HeaderParser.PATTERN_NF_FUNCTION.finditer(text)

        for idx, match in enumerate(matches):
            print(f"\nOccurance {idx}:")

            function_annotation = match.group(1).strip()
            function_declaration = match.group(2).strip()
            function_span = range(*match.span())

            function_namespace = self._find_enclosing_namespace(function_span)
            function_namespace = function_namespace.name if function_namespace else ""

            nf_functions.append(
                NFFunction(
                    annotation=function_annotation,
                    declaration=function_declaration,
                    span=function_span,
                    namespace=function_namespace
                )
            )

            print(f"\t1: {nf_functions[-1].annotation()}")
            print(f"\t\t-> {nf_functions[-1].annotation().attributes}")
            print(f"\t2: {nf_functions[-1].declaration()}")

        print("----------------------------------------------------------------------------------------"
              "---------------------------------------------------------------\n")
        return nf_functions

    def _find_classes_with_annotations(self, text: str):

        print("--------------------------------------------------------------------- Parsing NFClasses "
              "---------------------------------------------------------------")
        nf_classes = []

        matches = HeaderParser.PATTERN_NF_CLASS.finditer(text)
        for idx, match in enumerate(matches):
            print(f"\nOccurance {idx}:")

            class_annotation = match.group(1).strip()
            class_declaration = match.group(2).strip()

            if class_declaration.count('class') > 1:
                raise ParseException("Nested class are not allowed")

            class_span = range(*match.span())
            class_namespace = self._find_enclosing_namespace(class_span)
            class_namespace = class_namespace.name if class_namespace else ""

            nf_class = NFClass(
                annotation=class_annotation,
                declaration=class_declaration,
                span=class_span,
                namespace=class_namespace
            )
            nf_class.find_and_set_methods(funcs=self.nf_functions)
            nf_classes.append(nf_class)

            print(f"\t1: {nf_classes[-1].annotation()}")
            print(f"\t\t-> {nf_classes[-1].annotation().attributes}")
            print(f"\t2: {nf_classes[-1].name()}")

            for idx, method in enumerate(nf_classes[-1].methods()):
                print(f"\t\t\t method {idx}: {method.annotation()}")
                print(f"\t\t\t\t-> {method.annotation().attributes}")
                print(f"\t\t\t method {idx}: {method.declaration()}\n")

        print("----------------------------------------------------------------------------------------"
              "---------------------------------------------------------------\n")

        return nf_classes


    def _find_events_with_annotations(self, text: str):

        print("--------------------------------------------------------------------- Parsing Events "
              "---------------------------------------------------------------")
        nf_events = []

        matches = HeaderParser.PATTERN_NF_EVENT.finditer(text)
        for idx, match in enumerate(matches):
            print(f"\nOccurance {idx}:")

            event_annotation = match.group(1).strip()
            event_declaration = match.group(2).strip()

            if event_declaration.count('class') > 1:
                raise ParseException("Nested class are not allowed")

            if event_declaration.count('struct') > 1:
                raise ParseException("Nested structs are not allowed")

            event_span = range(*match.span())
            event_namespace = self._find_enclosing_namespace(event_span)
            event_namespace = event_namespace.name if event_namespace else ""



            nf_event = NFEvent(
                annotation=event_annotation,
                declaration=event_declaration,
                span=event_span,
                namespace=event_namespace
            )

            nf_events.append(nf_event)


            print(f"\t1: {nf_event.annotation()}")
            print(f"\t\t-> {nf_event.annotation().attributes}")
            print(f"\t2: {nf_event.declaration()}")

            print(f"\t3 Args: {nf_event.event_arg_names()}")


        print("----------------------------------------------------------------------------------------"
              "---------------------------------------------------------------\n")

        return nf_events

    def _find_enclosing_namespace(self, span: range) -> Optional[Namespace]:
        """Finds the namespace that encloses the given range
        :return: Enclosing namespace object
        """

        if not len(self.namespaces) > 0:
            return None

        # Sort namespace from small to large range.
        namespaces = sorted(self.namespaces, key=lambda dist: (dist.span.stop - dist.span.start))

        # We check if span is inside namespace
        # Because we sorted, we return when we find lowest enclosing namespace
        for namespace in namespaces:
            if range_subset(span, namespace.span):
                return namespace

        return None


if __name__ == '__main__':
    # ToDo: We support multiple ouput ports, by passing non-const references in function arguments and turn them into
    #  output ports -> set NonConstRefArgsAsOut. Other option is to really make them non-const references and mutate
    #  input. This is what Unreal Engine does when you set UPARAM(ref) (see:
    #  https://forums.unrealengine.com/t/how-can-i-write-a-c-blueprint-function-with-an-input-parameter-thats-pass-by
    #  -reference/328256/5). Unreal Engine then really does mutate input. Maybe make warning about it or require user
    #  to set extra flag but carry the outcomes

    # ToDo: Parsing Structs or Events breaks when we declar stuff in it that ends with }; before the actual }; of the
    #  structure: Like enum class {...}; or double m_member {1};
    # Fix it by using bracket counting.

    # ToDo: DisplayName in Annotations should be OverrideName

    try:
        file = "test.h"
        start = datetime.datetime.now()
        hparser = HeaderParser(file)
        end = datetime.datetime.now()


        print(f"Parsing took {(end - start).microseconds / 1000} ms.")

    except Exception as e:
        raise RuntimeError(f"Exception occured while parsing file '{file}'") from e

    # clear
    print("\n" * 100)

    for nf_function in hparser.nf_functions:
        try:
            nbuilder = FunctionNodeBuilder(nf_function)
            nbuilder()
            print(nbuilder.code)
        except Exception as e:
            raise CodeGenException(f"The above Exception occured during code generation for function: '{nf_function.name()}'") from e
        exit(0)
