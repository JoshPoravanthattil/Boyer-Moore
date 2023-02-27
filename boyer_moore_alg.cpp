/* C program for Boyer Moore Algorithm with 
   Good Suffix heuristic to find pattern in
   given text string */
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
# define NO_OF_CHARS 256
  
// preprocessing for strong good suffix rule
void preprocess_strong_suffix(int *shift, int *bpos,
                                char *pat, int m)
{
    // m is the length of pattern 
    int i=m, j=m+1;
    bpos[i]=j;
  
    while(i>0)
    {
        /*if character at position i-1 is not equivalent to
          character at j-1, then continue searching to right
          of the pattern for border */
        while(j<=m && pat[i-1] != pat[j-1])
        {
            /* the character preceding the occurrence of t in 
               pattern P is different than the mismatching character in P, 
               we stop skipping the occurrences and shift the pattern
               from i to j */
            if (shift[j]==0)
                shift[j] = j-i;
  
            //Update the position of next border 
            j = bpos[j];
        }
        /* p[i-1] matched with p[j-1], border is found.
           store the  beginning position of border */
        i--;j--;
        bpos[i] = j; 
    }
}
  
//Preprocessing for case 2
void preprocess_case2(int *shift, int *bpos,
                      char *pat, int m)
{
    int i, j;
    j = bpos[0];
    for(i=0; i<=m; i++)
    {
        /* set the border position of the first character of the pattern
           to all indices in array shift having shift[i] = 0 */ 
        if(shift[i]==0)
            shift[i] = j;
  
        /* suffix becomes shorter than bpos[0], use the position of 
           next widest border as value of j */
        if (i==j)
            j = bpos[j];
    }
}
  
// The preprocessing function for Boyer Moore's
// bad character heuristic
void badCharHeuristic( char *str, int size,
                        int badchar[NO_OF_CHARS])
{
    int i;
 
    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;
 
    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
        badchar[(int) str[i]] = i;
}

/*Search for a pattern in given text using
  Boyer Moore algorithm with Good suffix rule */
void search(char *text, char *pat)
{
    // s is shift of the pattern with respect to text
    int s=0, j;
    int m = strlen(pat);
    int n = strlen(text);
    int badchar[NO_OF_CHARS];
    int bpos[m+1], shift[m+1];
  
    //initialize all occurrence of shift to 0
    for(int i=0;i<m+1;i++) {
        shift[i]=0;
    } 
  
    //do preprocessing
    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pat, m, badchar);
    preprocess_strong_suffix(shift, bpos, pat, m);
    preprocess_case2(shift, bpos, pat, m);
  
    while(s <= n-m)
    {
  
        j = m-1;
  
        /* Keep reducing index j of pattern while characters of
             pattern and text are matching at this shift s*/
        while(j >= 0 && pat[j] == text[s+j])
            j--;
  
        /* If the pattern is present at the current shift, then index j
             will become -1 after the above loop */
        if (j<0)
        {
            printf("pattern occurs at shift = %d\n", s);
            s += shift[0];
        }
        else
            /*pat[i] != pat[s+j] so shift the pattern
              shift[j+1] times  */
            // s += shift[j+1];
            s += max(1, max(j - badchar[text[s + j]], shift[j+1]));
            // cout << "Values: " << 1 << " " << j - badchar[text[s + j]] << " " << shift[j+1] << "\n";
            // cout << "Max: " << max(1, max(j - badchar[text[s + j]], shift[j+1])) << "\n";
    }
  
}
  
//Driver 
int main()
{
    char text[] = "2384792834792837492837492834792478294783ABAAAABAACD23948729834792837492834792837492874928374";
    char pat[] = "ABA";
    search(text, pat);
    return 0;
}