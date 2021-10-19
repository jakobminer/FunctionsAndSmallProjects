import java.util.Stack;

import static java.util.Arrays.sort;

public class NFA {
    int startState;
    int endState;
    String transitions;

    NFA(int start, int end, char input){
        startState = start;
        endState = end;
        transitions = "";
        addTransition(start, end, input);

    }

    NFA(int start, int end, String newTransitions){
        startState = start;
        endState = end;
        transitions = newTransitions;

    }

    void addTransition(int start, int end, char input){
        transitions = transitions + "(q" + start + ", " + input + ") -> q" + end + "\n";
    }

    public void printNFA(){
        String[] transitionsArray = transitions.split("\n");
        sort(transitionsArray);
        int i;

        System.out.println("Start: q" + startState);
        System.out.println("Accept: q" + endState);
        for (i=0; i<transitionsArray.length; i++){
            System.out.println(transitionsArray[i]);
        }
    }
}