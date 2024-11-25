from datetime import datetime

from typing import Generic, TypeVar
T = TypeVar('T')  # Typvariablen-Definition

class DataType(Generic[T]):
    value: T

    def __init__(self, value: T) -> None:
        self.value = value
      #  print("1")

    def __str__(self) -> str:
        return str(self.value)
    


class TimedDataType(DataType[T]):
    timestamp: datetime | None

    def __init__(self, value: T, timestamp: datetime | None = None) -> None:
      #  print("2")
        super().__init__(value)
        self.timestamp = timestamp
        

class BoolType(TimedDataType[bool]):
    def __init__(self, value: bool, timestamp: datetime | None = None) -> None:
       # print("2.1")

        super().__init__(value, timestamp)

    def __str__(self) -> str:
        #print( str(self.value).lower())
        return str(self.value).lower()
    

class StrType(TimedDataType[str]):
    def __init__(self, value: str, timestamp: datetime | None = None) -> None:
      #  print("class strtype")
        super().__init__(value, timestamp)


class NullType(DataType[None]):
    def __init__(self) -> None:
        #print("4")
        super().__init__(None)

    def __str__(self) -> str:
        return "Null"
    
class NumType(TimedDataType[int | float]):
    def __init__(self, value: int | float, timestamp: datetime | None = None) -> None:
        super().__init__(value, timestamp)

    def __str__(self) -> str:
       # if self.value.is_integer():
       #     return str(int(self.value))
        return str(self.value)
    

class TimeType(TimedDataType[datetime]):
    def __init__(self, value: datetime, timestamp: datetime | None = None) -> None:
       # print("6")
        super().__init__(value, timestamp)

    @staticmethod
    def now():
        return TimeType(datetime.now())

    def __str__(self) -> str:
        return str(self.value)
    


class ListType:
    items: list[DataType]

    def __init__(self, items: list[DataType] | None = None) -> None:
        if items is None:
            self.items = []
        else:
            self.items = items

    def length(self) -> int:
        return len(self.items)

    def __str__(self) -> str:
       # return f"[{", ".join([str(x) for x in self.items])}]"
        return f"[{', '.join([str(x) for x in self.items])}]"