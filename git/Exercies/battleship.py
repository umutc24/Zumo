'''Meta interview, interview question 1, battleship
   Umut Ceylan '''

import numpy as np
board = np.zeros((8,8), dtype= int)
board[0,3] = 1
board[0,4] = 1
board[0,5] = 1
print(board)


def bomb_square_v1(x, y):
    if board[x,y] == 1:
        print("Hit!")
        x +=1
        if board[x,y] == 1:
            print(f"Your ship coordinates are: ({x-1}, {y}), ({x},{y}), ({x+1},{y}).")
        else:
            x -= 1
            print(f"Your ship coordinates are: ({x}, {y}), ({x},{y+1}), ({x},{y+2}).")
    else:
        print("Miss!")
    x += 3
    if board[x, y] == 1:
        print("Hit!")
        x += 1
        if board[x,y] ==1:
            print(f"Your ship coordinates are: ({x - 1}, {y}), ({x},{y}), ({x + 1},{y}).")
        else:
            x -= 1
            print(f"Your ship coordinates are: ({x}, {y}), ({x},{y + 1}), ({x},{y + 2}).")
    else:
        print("Miss!")
    x += 3
    if board[x, y] == 1:
        print("Hit!")
        x +=1
        if board[x,y] ==1:
            print(f"Your ship coordinates are: ({x - 1}, {y}), ({x},{y}), ({x + 1},{y}).")
        else:
            x -= 1
            print(f"Your ship coordinates are: ({x}, {y}), ({x},{y + 1}), ({x},{y + 2}).")
    else:
        print("Miss!")
    x += 1
    if board[x,y] == 1:
        print(f"Your ship coordinates are: ({x}, {y}), ({x},{y+1}), ({x},{y+2}).")

    x -= 7
    y += 3
    print(f"After checking x=0 vertical lines you're here: {x}, {y}")
    if board[x,y] == 1:
        print("Hit!")
        y += 2
        if board[x,y] == 1:
            print(f"Your ship coordinates are: ({x}, {y-2}), ({x},{y-1}), ({x},{y}).")
        else:
            y -= 2
            print(f"Your ship coordinates are: ({x}, {y-2}), ({x},{y-1}), ({x},{y}).")
    else:
        print("Miss!")
    x += 3
    if board[x,y] == 1:
         print("Hit!")

# After this do the vertical one from (0,0) to (8,8) and then try from (0,3) to (3,7) and so on

# def bomb_square_v2(x,y):


bomb_square_v1(0,0)
