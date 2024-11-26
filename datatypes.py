from datetime import datetime


class BoolType():
    def __init__(self, value: bool, timestamp: datetime) -> None:
        self.value = value
        self.timestamp = timestamp

    def __str__(self) -> str:
        return str(self.value).lower()

    
class StrType():
    def __init__(self, value: str, timestamp: datetime) -> None:
        self.value = value
        self.timestamp = timestamp

    def __str__(self) -> str:
        return str(self.value).lower() 


class NumType():
    def __init__(self, value: int | float, timestamp: datetime) -> None:
        self.value = value
        self.timestamp = timestamp

    def __str__(self) -> str:
        return str(self.value)
    

class TimeType():
    def __init__(self, value: datetime, timestamp: datetime) -> None:
        self.value = value
        self.timestamp = timestamp

    def __str__(self) -> str:
        return str(self.value)


class ListType:

    def __init__(self, items: list, timestamp: datetime) -> None:
        if items is None:
            self.items = []
        else:
            self.items = items

        self.timestamp = timestamp

    def __str__(self) -> str:
        return f"[{', '.join([str(x) for x in self.items])}]"
    
    

class NullType():
    def __init__(self) -> None:
        self.value = None  # Speichert None

    def __str__(self) -> str:
        return "Null"
    



#class ListType:
#    items: list[DataType]

#    def __init__(self, items: list[DataType], timestamp: datetime) -> None:
#        if items is None:
         #   self.items = []
#        else:
#            self.items = items

 #   def __str__(self) -> str:
 #       return f"[{', '.join([str(x) for x in self.items])}]"
    

#from typing import Generic, TypeVar
#T = TypeVar('T')  # Typvariablen-Definition ~Platzhalter, Notwenidg, damit Typ bem anlegen eindeutig definiert wird

#class DataType(Generic[T]):
#    value: T

#    def __init__(self, value: T) -> None: 
#        self.value = value

 #   def __str__(self) -> str:
 #       return str(self.value)

#class BoolType(DataType[bool]):
#    def __init__(self, value: bool, timestamp: datetime) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp

 #   def __str__(self) -> str:
  #      return str(self.value).lower()
    

#class StrType(DataType[str]):
#    def __init__(self, value: str, timestamp: datetime) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp

#    def __str__(self) -> str:
#        return str(self.value).lower()



#class NumType(DataType[int | float]):
#    def __init__(self, value: int | float, timestamp: datetime) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp

#    def __str__(self) -> str:
#        return str(self.value)


#class TimeType(DataType[datetime]):
#    def __init__(self, value: datetime, timestamp: datetime | None = None) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp

#    @staticmethod
#    def now():
#        return TimeType(datetime.now())

#    def __str__(self) -> str:
#        return str(self.value)


#class ListType:
#    items: list[DataType]

#    def __init__(self, items: list[DataType] | None = None) -> None:
#        if items is None:
#            self.items = []
#        else:
#            self.items = items

#    def __str__(self) -> str:
#        return f"[{', '.join([str(x) for x in self.items])}]"

#class NumType(TimedDataType[int | float]):
#    def __init__(self, value: int | float, timestamp: datetime | None = None) -> None:
#        super().__init__(value, timestamp)
#
#    def __str__(self) -> str:
#        return str(self.value)


#class StrType(TimedDataType[str]):
#    def __init__(self, value: str, timestamp: datetime | None = None) -> None:
#        super().__init__(value, timestamp)
#
#    def __str__(self) -> str:
#        return str(self.value).lower()

#class TimedDataType(DataType[T]):
#    timestamp: datetime | None
#
#    def __init__(self, value: T, timestamp: datetime | None = None) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp

#class BoolType(TimedDataType[bool]):
#    def __init__(self, value: bool, timestamp: datetime | None = None) -> None:
#        super().__init__(value, timestamp)

#    def __str__(self) -> str:
#        return str(self.value).lower()


#class TimedDataType(DataType[T]):
#    timestamp: datetime
#
#    def __init__(self, value: T, timestamp: datetime) -> None:
#        super().__init__(value)
#        self.timestamp = timestamp
        


    
#class BoolType(TimedDataType[bool]):
#    def __init__(self, value: bool, timestamp: datetime) -> None:
#        super().__init__(value, timestamp)

#    def __str__(self) -> str:
#        return str(self.value).lower()


#class TimeType(TimedDataType[datetime]):
#    def __init__(self, value: datetime, timestamp: datetime | None = None) -> None:
#        super().__init__(value, timestamp)

#    @staticmethod
#    def now():
#        return TimeType(datetime.now())

#    def __str__(self) -> str:
#        return str(self.value)


#class StrType(TimedDataType[str]):
#    def __init__(self, value: str, timestamp: datetime) -> None:
#        super().__init__(value, timestamp)

#    def __str__(self) -> str:
#        return str(self.value).lower()


    

    
#class NumType(TimedDataType[int | float]):
#    def __init__(self, value: int | float, timestamp: datetime) -> None:
#        super().__init__(value, timestamp)

#    def __str__(self) -> str:
#        return str(self.value)


#class TimeType():
 #   def __init__(self, value: datetime, timestamp: datetime) -> None:
  #      self.value = value
   #     self.timestamp = timestamp

   # @staticmethod
   # def now():
   #     return TimeType(datetime.now(), datetime.now())

    #def __str__(self) -> str:
     #   return str(self.value)