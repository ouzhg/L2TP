PROJECT (ouzhg)

AUX_SOURCE_DIRECTORY (. ouzhg_SRCS)

ADD_LIBRARY(Common struct.cpp utility.cpp
			lex.cpp parse.cpp
			Cabalar.cpp HengZhang.cpp prenex.cpp)
