from datatypes import DataType , ListType , NullType


class SymbolTable:
    __symbols: dict[str, DataType | ListType] = {}


    def get_variable_value(self, variable_name: str) -> DataType | ListType:

        symbol = self.__symbols.get(variable_name) #veruscht Wert von angegeben Variabele aus dem dict von der classe SymbolTable zu lesen
        if symbol is None:
            return NullType() 
        return symbol


    def set_variable_value(self, variable_name: str, value: DataType | ListType) -> None:
        self.__symbols[variable_name] = value