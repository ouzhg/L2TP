FUNCTION (ADD_TEST_SELF name path)

	ADD_EXECUTABLE (${name} ${path})
	TARGET_LINK_LIBRARIES (${name} Common)
	ADD_TEST(NAME ${name} 
			 WORKING_DIRECTORY ${ROOT_PATH} 
			 COMMAND ${EXECUTABLE_OUTPUT_PATH}/${name}
			)

ENDFUNCTION()

ADD_TEST_SELF(0.sample 0.sample/sample.cpp)

ADD_TEST_SELF(2.parse.1.atom.0.single 2.parse/1.atom.0.single.cpp)
ADD_TEST_SELF(2.parse.1.atom.1.clause 2.parse/1.atom.1.clause.cpp)
ADD_TEST_SELF(2.parse.1.atom.2.vari.1 2.parse/1.atom.2.vari.1.cpp)
ADD_TEST_SELF(2.parse.1.atom.2.vari.2 2.parse/1.atom.2.vari.2.cpp)
ADD_TEST_SELF(2.parse.1.atom.3.func.1.single 2.parse/1.atom.3.func.1.single.cpp)
ADD_TEST_SELF(2.parse.1.atom.3.func.2.clause 2.parse/1.atom.3.func.2.clause.cpp)
ADD_TEST_SELF(2.parse.1.atom.3.func.3.vari   2.parse/1.atom.3.func.3.vari.cpp)
ADD_TEST_SELF(2.parse.1.atom.3.func.4.double 2.parse/1.atom.3.func.4.double.cpp)
ADD_TEST_SELF(2.parse.1.atom.4.cascade 2.parse/1.atom.4.cascade.cpp)
ADD_TEST_SELF(2.parse.2.nega.0.single 2.parse/2.nega.0.single.cpp)
ADD_TEST_SELF(2.parse.2.nega.1.double 2.parse/2.nega.1.double.cpp)
ADD_TEST_SELF(2.parse.2.nega.2.trible 2.parse/2.nega.2.trible.cpp)
ADD_TEST_SELF(2.parse.2.nega.3.clause-double 2.parse/2.nega.3.clause-double.cpp)
ADD_TEST_SELF(2.parse.2.nega.4.clause-trible 2.parse/2.nega.4.clause-trible.cpp)
ADD_TEST_SELF(2.parse.3.conj.0.single  2.parse/3.conj.0.single.cpp)
ADD_TEST_SELF(2.parse.3.conj.1.cascade 2.parse/3.conj.1.cascade.cpp)
ADD_TEST_SELF(2.parse.3.conj.2.line    2.parse/3.conj.2.line.cpp)
ADD_TEST_SELF(2.parse.4.disj.0.single  2.parse/4.disj.0.single.cpp)
ADD_TEST_SELF(2.parse.4.disj.1.cascade 2.parse/4.disj.1.cascade.cpp)
ADD_TEST_SELF(2.parse.4.disj.2.line    2.parse/4.disj.2.line.cpp)
ADD_TEST_SELF(2.parse.5.impl.0.single  2.parse/5.impl.0.single.cpp)
ADD_TEST_SELF(2.parse.5.impl.1.cascade 2.parse/5.impl.1.cascade.cpp)
ADD_TEST_SELF(2.parse.5.impl.2.line    2.parse/5.impl.2.line.cpp)
ADD_TEST_SELF(2.parse.6.univ.0.single  2.parse/6.univ.0.single.cpp)
ADD_TEST_SELF(2.parse.6.univ.1.cascade  2.parse/6.univ.1.cascade.cpp)
ADD_TEST_SELF(2.parse.7.exis.0.single  2.parse/7.exis.0.single.cpp)
ADD_TEST_SELF(2.parse.7.exis.1.cascade  2.parse/7.exis.1.cascade.cpp)
ADD_TEST_SELF(2.parse.7.exis.2.mix.behind  2.parse/7.exis.2.mix.behind.cpp)
ADD_TEST_SELF(2.parse.7.exis.2.mix.front   2.parse/7.exis.2.mix.front.cpp)

ADD_TEST_SELF(A.cabalar.0.normal.l1 5.cabalar/A.cabalar.0.normal.l1.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.l2 5.cabalar/A.cabalar.0.normal.l2.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.l3 5.cabalar/A.cabalar.0.normal.l3.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.l4 5.cabalar/A.cabalar.0.normal.l4.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.l5 5.cabalar/A.cabalar.0.normal.l5.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.r1 5.cabalar/A.cabalar.0.normal.r1.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.r2 5.cabalar/A.cabalar.0.normal.r2.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.r3 5.cabalar/A.cabalar.0.normal.r3.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.r4 5.cabalar/A.cabalar.0.normal.r4.cpp)
ADD_TEST_SELF(A.cabalar.0.normal.r5 5.cabalar/A.cabalar.0.normal.r5.cpp)