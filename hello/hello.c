/**
 * @file hello.c
 * Betriebssysteme Hello World File.
 * Beispiel 0
 *
 * @author Bernd Petrovitsch <bernd.petrovitsch@technikum-wien.at>
 * @date 2005/02/22
 *
 * @version 470 
 *
 * @todo Test it more seriously and more complete.
 * @todo Review it for missing error checks.
 * @todo Review it and check the source against the rules at
 *       https://cis.technikum-wien.at/documents/bic/2/bes/semesterplan/lu/c-rules.html
 *
 */

/*
 * -------------------------------------------------------------- includes --
 */

#include <stdio.h>

/*
 * --------------------------------------------------------------- defines --
 */

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

/*
 * ------------------------------------------------------------- functions --
 */

/**
 *
 * \brief The most minimalistic C program
 *
 * This is the main entry point for any C program.
 *
 * \param argc the number of arguments
 * \param argv the arguments itselves (including the program name in argv[0])
 *
 * \return always "success"
 * \retval 0 always
 *
 */
int main(int argc,
         const char *argv[])
{
    /* prevent warnings regarding unused params */
    argc = argc;
    argv = argv;

    printf("Hello world!\n");
    return 0;
}

/*
 * =================================================================== eof ==
 */
