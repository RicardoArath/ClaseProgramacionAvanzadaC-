#include <iostream>
#include <random>
#include <conio.h>

using namespace std;

bool isDebugMode = false;

string debug(string msg) {
    if (isDebugMode)
        cout << "[DEBUG]: " << msg << endl;
    return msg;
}

int main() {
    try{
    cout << "Welcome to Rock-Paper-Scissors!" << endl;

    // Game logic would go here
    cout << "Type any key to start the game." << endl;
    _getch();

    int playerScore = 0;
    int computerScore = 0;
    cout << "Initial scores - Player: " + to_string(playerScore) + ", Computer: " + to_string(computerScore) << endl;

    // Simple game loop
    for (int round = 1; round <= 3; round++) {
        cout << "Round " << round << ": Enter rock, paper, or scissors by typing [1] for rock, [2] for paper, or [3] for scissors: ";
        int playerInt;
        cin >> playerInt;

        while(cin.fail() || playerInt < 1 || playerInt > 3) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Descartar la entrada inválida
            cout << "Invalid input. Please enter [1] for rock, [2] for paper, or [3] for scissors: ";
            cin >> playerInt;
        }

        switch(playerInt) {
            case 1:
                cout << "ROCK\n";
                cout << "    _______\n";
                cout << "---'   ____)\n";
                cout << "      (_____)\n";
                cout << "      (_____)\n";
                cout << "      (____)\n";
                cout << "---.__(___)\n";
                break;
            case 2:
                cout << "PAPER\n";
                cout << "     _______\n";
                cout << "---'    ____)____\n";
                cout << "           ______)\n";
                cout << "          _______)\n";
                cout << "         _______)\n";
                cout << "---.__________)\n";
                break;
            case 3:
                cout << "SCISSORS\n";
                cout << "    _______\n";
                cout << "---'   ____)____\n";
                cout << "          ______)\n";
                cout << "       __________)\n";
                cout << "      (____)\n";
                cout << "---.__(___)\n";
                break;
        }

        string playerChoice;

        if (playerInt == 1) playerChoice = "rock";
        if (playerInt == 2) playerChoice = "paper";
        if (playerInt == 3) playerChoice = "scissors";
        debug("Player choice: " + playerChoice);

        cout << "Player chose: " << playerChoice << endl;

        // Generate computer choice
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 2);
        int computerInt = dis(gen);

        switch(computerInt) {
            case 0:
                cout << "ROCK\n";
                cout << "    _______\n";
                cout << "---'   ____)\n";
                cout << "      (_____)\n";
                cout << "      (_____)\n";
                cout << "      (____)\n";
                cout << "---.__(___)\n";
                break;
            case 1:
                cout << "PAPER\n";
                cout << "     _______\n";
                cout << "---'    ____)____\n";
                cout << "           ______)\n";
                cout << "          _______)\n";
                cout << "         _______)\n";
                cout << "---.__________)\n";
                break;
            case 2:
                cout << "SCISSORS\n";
                cout << "    _______\n";
                cout << "---'   ____)____\n";
                cout << "          ______)\n";
                cout << "       __________)\n";
                cout << "      (____)\n";
                cout << "---.__(___)\n";
                break;
        }

        string computerChoice;
        if (computerInt == 0) computerChoice = "rock";
        if (computerInt == 1) computerChoice = "paper";
        if (computerInt == 2) computerChoice = "scissors";
        debug("Computer choice (int): " + to_string(computerInt));
        debug("Computer choice (string): " + computerChoice);

        cout << "Computer chose: " << computerChoice << endl;

        // Determine winner
        if (playerChoice == computerChoice) {
            cout << "It's a tie!" << endl;
        }

        // Determine tier
        if (playerChoice == "rock" && computerChoice == "scissors" ||
            playerChoice == "paper" && computerChoice == "rock" ||
            playerChoice == "scissors" && computerChoice == "paper"){
            cout << "You win this round!" << endl;
            playerScore++;
        }
        
        if (playerChoice == "rock" && computerChoice == "paper" ||
            playerChoice == "paper" && computerChoice == "scissors" ||
            playerChoice == "scissors" && computerChoice == "rock") { //Determine computer win
            cout << "Computer wins this round!" << endl;
            computerScore++;
        }

        cout << "Current scores - Player: " + to_string(playerScore) + ", Computer: " + to_string(computerScore) << endl;
    }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}