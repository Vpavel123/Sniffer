class ConsoleState
{
public:
    virtual void display(char choice) = 0;
    virtual void ConsoleHandle() = 0;
};

class MenuConsole : public ConsoleState
{
public:
    void display(char choice) override;
    void ConsoleHandle() override;
};

void MenuConsole::display(char choice){
    switch (choice)
    {
    case '1':
        
        break;
    
    default:
        break;
    }
}