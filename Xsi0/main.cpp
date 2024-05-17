#include <iostream>
#include <array>

class XsiO
{
public:
    enum class square
    {
        X,
        O,
        empty
    };

    XsiO()
    {
        for (size_t i = 0; i < board.size(); i++)
        {
            for (size_t j = 0; j < board[i].size(); j++)
            {
                board[i][j] = square::empty;
            }
        }
        next = square::X;
    }

    void move(size_t rand, size_t col)
    {
        board[rand][col] = next;
        if (next == square::X)
        {
            next = square::O;
        }
        else
        {
            next = square::X;
        }
    }

    char c(square s) const
    {
        switch (s)
        {
        case square::O: return 'O';
        case square::X: return 'X';
        case square::empty: return ' ';
        }
        return ' ';
    }

    friend std::ostream & operator<<(std::ostream & os, XsiO const & game)
    {
        for (size_t i = 0; i < game.board.size(); i++)
        {
            for (size_t j = 0; j < game.board[i].size(); j++)
            {
                os << game.c(game.board[i][j]) << '|';
            }
            os << '\b' << ' ' << std::endl;
            if(i < 2) os << "-+-+-" << std::endl;
        }
        return os;
    }

    bool finished()
    {
        // diagonalele
        if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != square::empty) ||
            (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != square::empty))
        {
            std::cout << "Jocul a fost castigat!" << std::endl;
                return true;
        }

        // liniile și coloanele
        for (int i = 0; i < 3; i++)
        {
            bool row_win = true;
            bool col_win = true;
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != board[i][0] || board[i][j] == square::empty) row_win = false;
                if (board[j][i] != board[0][i] || board[j][i] == square::empty) col_win = false;
            }
            if (row_win || col_win)
            {
                std::cout << "Jocul a fost castigat!" << std::endl;
                    return true;
            }
        }

        // tabla de joc este completă
        bool full_board = true;
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                if (board[i][j] == square::empty)
                {
                    full_board = false;
                    break;
                }
            }
            if (!full_board)
            {
                break;
            }
        }
        if (full_board)
        {
            std::cout << "Jocul s-a incheiat cu remiza." << std::endl;
                return true;
        }

        return false;
    }
private:
    std::array<std::array<square, 3>, 3> board;
    square next;
};


int main()
{
    XsiO game;
    while (!game.finished())
    {
        int x;
        int y;
        std::cin >> x >> y;
        game.move(x, y);
        std::cout << game << std::endl;
    }
}
