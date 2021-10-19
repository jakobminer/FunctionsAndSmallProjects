Jakob Miner
	jakob.miner@wsu.edu
CS317 Automata and Formal Languages
Prof. Farhana
--------------------------------------
This project is a RE to NFA converter
	it accepts a file of properly-formatted, line-seperated, postfix-notation regex's
	it converts them to a list of state transitions describing an NFA
-------------------------------------------
to build, all you should need is to enter
	javac Main.java NFA.java

and run with
	java Main <filename>
---------------------------------------
the contents of this archive should be
	Main.java
		containts the main program. reads file, converts REs to NFAs using a stack.
	NFA.java
		NFA class allows creation of an NFA object and associated NFA methods
	README.txt
		this file