import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Stack;

public class Main{

    public static void main(String[] args) throws FileNotFoundException {
        File inputFile = new File(args[0]);
        Scanner sc = new Scanner(inputFile);

        Stack<NFA> NFAStack = new Stack<NFA>();

        while (sc.hasNextLine()){

            int i;
            int unusedState = 1;
            String regEx = sc.nextLine();
            if (!regEx.isEmpty()){
                for (i=0; i<regEx.length(); i++){
                    char input = regEx.charAt(i);
                    if (input=='|'){
                        NFA nfa1 = NFAStack.pop();
                        NFA nfa2 = NFAStack.pop();
                        NFA nfaNew = new NFA(unusedState++, unusedState++, nfa1.transitions + nfa2.transitions);
                        nfaNew.addTransition(nfaNew.startState, nfa1.startState, 'E');
                        nfaNew.addTransition(nfaNew.startState, nfa2.startState, 'E');
                        nfaNew.addTransition(nfa1.endState, nfaNew.endState, 'E');
                        nfaNew.addTransition(nfa2.endState, nfaNew.endState, 'E');
                        NFAStack.push(nfaNew);
                    }
                    else if (input == '&'){
                        NFA nfa2 = NFAStack.pop();
                        NFA nfa1 = NFAStack.pop();
                        NFA nfaNew = new NFA(nfa1.startState, nfa2.endState, nfa1.transitions + nfa2.transitions);
                        nfaNew.addTransition(nfa1.endState, nfa2.startState, 'E');
                        NFAStack.push(nfaNew);
                    }
                    else if (input=='*'){
                        NFA nfa = NFAStack.pop();
                        int newState = unusedState++;
                        nfa.addTransition(newState, nfa.startState, 'E');
                        nfa.addTransition(nfa.endState, newState, 'E');
                        nfa.startState = newState;
                        nfa.endState = newState;
                        NFAStack.push(nfa);
                    }
                    else {
                        NFA newNFA = new NFA(unusedState++, unusedState++, input);
                        NFAStack.push(newNFA);
                    }
                }
                NFA finalNFA = NFAStack.pop();
                System.out.println(regEx);
                finalNFA.printNFA();
                System.out.println();
            }
        }
        sc.close();
    }
}