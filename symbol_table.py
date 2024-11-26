from datatypes import BoolType, StrType, NumType, TimeType, ListType, NullType


class SymbolTable:
    symbols: dict[str, BoolType | StrType | NumType | TimeType | ListType | NullType] = {}

    def get_variable_value(self, variable_name: str) -> BoolType | StrType | NumType | TimeType | ListType | NullType:

        symbol = self.symbols.get(variable_name) #veruscht Wert von angegeben Variabele aus dem dict von der classe SymbolTable zu lesen
        if symbol is None:
            return NullType() 
        return symbol


    def set_variable_value(self, variable_name: str, value: BoolType | StrType | NumType | TimeType | ListType | NullType) -> None:
        self.symbols[variable_name] = value






#from datatypes import ListType , NullType ,DataType


#class SymbolTable:
#    __symbols: dict[str, DataType | ListType] = {}


#    def get_variable_value(self, variable_name: str) -> DataType | ListType:

#        symbol = self.__symbols.get(variable_name) #veruscht Wert von angegeben Variabele aus dem dict von der classe SymbolTable zu lesen
#        if symbol is None:
#            return NullType() 
#        return symbol


#    def set_variable_value(self, variable_name: str, value: DataType | ListType) -> None:
#        self.__symbols[variable_name] = value

