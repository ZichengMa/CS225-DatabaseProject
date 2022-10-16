BT : B+Test.cpp
	g++  -g B+Test.cpp -o BT
BM : BMultiTest.cpp
	g++  -g BMultiTest.cpp -o BM
Simulator: Acsctrl.o FBH.o Calc.o DataInterface.o Calc_p.o DateTime.o Comparison.o DataStorage.o Record.o RecordPriority.o Utility.o Hospital.o
	g++ -g Acsctrl.o FBH.o Calc.o DataInterface.o Calc_p.o DateTime.o Comparison.o DataStorage.o Record.o RecordPriority.o Utility.o Hospital.o Simulator.cpp Simulator_test.cpp -o Simulator
DataInterface.o : DataInterface.cpp
	g++ -c  -g DataInterface.cpp -o DataInterface.o
Acsctrl.o: Acsctrl.cpp
	g++ -g -c Acsctrl.cpp -o Acsctrl.o 
Calc.o: Calc.cpp
	g++ -g -c Calc.cpp -o Calc.o 
Calc_p.o: Calc_p.cpp
	g++ -g -c Calc_p.cpp -o Calc_p.o
DateTime.o: DateTime.cpp
	g++ -g -c DateTime.cpp -o DateTime.o
Comparison.o: Comparison.cpp
	g++ -g -c Comparison.cpp -o Comparison.o
DataStorage.o: DataStorage.cpp
	g++ -g -c DataStorage.cpp -o DataStorage.o
Hospital.o: Hospital.cpp
	g++ -g -c Hospital.cpp -o Hospital.o
Record.o: Record.cpp
	g++ -g -c Record.cpp -o Record.o
RecordPriority.o: RecordPriority.cpp
	g++ -g -c RecordPriority.cpp -o RecordPriority.o
Utility.o: Utility.cpp
	g++ -g -c Utility.cpp -o Utility.o
clean:
	rm *.o
