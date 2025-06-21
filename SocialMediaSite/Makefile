
app: 
	g++ -std=c++17 -c graph.cpp 
	g++ -std=c++17 -c userSimulate.cpp 
	g++ -std=c++17 -c postUpdate.cpp 
	g++ -std=c++17 -c readPost.cpp 
	g++ -std=c++17 -c utils.cpp 
	g++ -std=c++17 -c main.cpp 
	g++ -std=c++17 -o main main.o utils.o graph.o userSimulate.o postUpdate.o readPost.o


verify: 
	python3 verification/parseLogFiles.py 
	python3 verification/graphVerification.py 

clean: 
	rm -f *.o *.log main verification/*.csv 
	