#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define move 8 

int valid(char **board, int x, int y, int m, int n)
{
    // function to check if a move is valid
    if(x>=0 && y>=0 && x<m && y<n && board[x][y]=='.')
    {
        return 1;
    }
    return 0;
}

int knight(char **board, int currentX, int currentY, int moveNum, int xdir[], int ydir[], int m, int n) //first move para: board, currentX, currentY, move#, total possible X, total possibleY, rows, columns
{
    int i,j;
    int v;
    if(moveNum==m*n)
        return 1;
    for(i=0; i<m; ++i)
    {
        for(j=0; j<n; ++j)
        {
            printf("%c",board[i][j]);
            fflush(stdout);
        }
        printf("\n");
    }
    printf("\n\n");
    for(i=0; i<8; ++i)
    {
    //    printf("%d %d\n",currentX, currentY);
        
        v = valid(board, currentX + xdir[i], currentY + ydir[i], m, n);
        if(v == 1)
        {
            currentX += xdir[i];
            currentY += ydir[i];
            board[currentX][currentY]=moveNum + '0';
            if(knight(board, currentX, currentY, moveNum + 1, xdir, ydir, m, n) == 1)
                return 1;
            else
            {
                board[currentX][currentY]='.';
                currentX -= xdir[i];
                currentY -= ydir[i];
            }
                
        }
        
    }
    //printf("\n\n\n");
    return 0;
}





int main(int argc, char*argv[])
{
    if(argc!=3)
    {
        fprintf(stderr, "ERROR: invalid arguments\n");
        return EXIT_FAILURE;
    }
    int m, n, i, j;
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    
    if(m<3 || n<3)
    {
        fprintf(stderr, "ERROR: invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    char **board = NULL;
    board = (char **)calloc(m, sizeof(char*));
    for(i=0; i<m; ++i)
    {
        board[i] = (char *)calloc(n, sizeof(char));
    }
    int currentX = 0;
    int currentY = 0;
    
    for(i=0; i<m; ++i)
    {
        for(j=0; j<n; ++j)
        {
            board[i][j] = '.';
        }
    }
    board[currentX][currentY]='k';
    
    pid_t currentProcessID;
    currentProcessID = getpid();
    printf("PID %d: Solving the knight's tour problem for a %dx%d board\n",currentProcessID,m,n);
    printf("PID %d: 2 moves possible after move #1\n",currentProcessID);
    fflush(stdout);
    int xdir[move] = { 2,2,1,1,-2,-2,-1,-1 };
    int ydir[move] = { 1,-1,2,-2,1,-1,2,-2 };
//    board[2][1]='k';
    int first = knight(board, 0, 0, 1, xdir, ydir, m, n); //first move para: board, currentX, currentY, move#, total possible X, total possibleY, rows, columns
    
    printf("Successfully terminated with return value %d \n", first);
    
    
    return EXIT_SUCCESS;
}