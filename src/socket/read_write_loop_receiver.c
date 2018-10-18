/**
 * Projet1 - Reseaux Informatiques
 * Octobre 2018
 *
 * Auteurs : Brieuc de Voghel   &   Nicolas van de Walle
 * NOMA    : 59101600           &   27901600
 *
 * Contenu repris et complete de l'exercice preparatoire au projet : https://inginious.info.ucl.ac.be/course/LINGI1341/envoyer-et-recevoir-des-donnees
 * Réalisé avec l'aide des sites suivants : https://github.com/Donaschmi/LINGI1341/blob/master/Inginious/Envoyer_et_recevoir_des_donn%C3%A9es/read_write_loop.c
 */


#include "read_write_loop_receiver.h"
#include "../packet/packet.h" // MAX_PAYLOAD_SIZE

/**
 * Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */
void read_write_loop_receiver(int sfd, stack_t *receivingStack, int outputFileDescriptor) {
    int getOut = 0;

    char stdInBuffer[MAX_PAYLOAD_SIZE];
    char sfdBuffer[MAX_PAYLOAD_SIZE];

    fd_set fdSet;

    struct timeval timeout;
    timeout.tv_sec = 10000;
    timeout.tv_usec = 0;

    //Unuseful but inginious needs to go through the warnings
    int written;
    int justRead;

    while (getOut == 0) {
        // Reset everything for new iteration of the loop
        memset(stdInBuffer, 0, MAX_PAYLOAD_SIZE);
        memset(sfdBuffer, 0, MAX_PAYLOAD_SIZE);

        justRead = 0;
        written = 0;

        FD_ZERO(&fdSet);
        FD_SET(0, &fdSet);
        FD_SET(sfd, &fdSet);

        select(sfd + 1, &fdSet, NULL, NULL, &timeout);

        if (FD_ISSET(sfd, &fdSet)) {
            justRead = read(sfd, sfdBuffer, MAX_PAYLOAD_SIZE);
            if (justRead == 0){
                break;
            }else if (justRead != -1) {
                written = write(1, sfdBuffer, justRead);
                if(written == -1) {
                    perror("Failed to write on stdout");
                }
            }
        }

        if (feof(stdin) != 0) {
            getOut = 1;
        }

    }
}