#include <vector>
#include <array>
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <functional>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define RANDOM_SEED 581259915
#define EXACT_VALUE 0
#define LOWER_BOUND 1
#define UPPER_BOUND 2
#define HASH_TABLE_SIZE 10485760
#define QUIET_MOVE_PRIORITY 51
//-1 to disable
#define QUIET_STATIC_DEPTH 0
#define MAX_DEPTH 5
#define NULL_MOVE_DEPTH -1
#define KILLER_PRIORITY_BONUS 2
#define KILLER_PRIORITY_BONUS_QUIESCENCE 10
#define ITERATIVE_BEST_PRIORITY 25

bool printTimeStats = true;
bool printTotalTimeStats = true;

int maxDepth = MAX_DEPTH;

unsigned long long int bitSequences[781];

bool slide[7] = {
    false, false, false, true, true, true, false
};
int numDirections[7] = {
    0,0,8,4,4,8,8
};
int offsets[7][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {-21,-19,-12,-8,8,12,19,21},
    {-11,-9,9,11,0,0,0,0},
    {-10,-1,1,10,0,0,0,0},
    {-11,-10,-9,-1,1,9,10,11},
    {-11,-10,-9,-1,1,9,10,11}
};

int mailbox[120] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1,0,1,2,3,4,5,6,7,-1,
        -1,8,9,10,11,12,13,14,15,-1,
        -1,16,17,18,19,20,21,22,23,-1,
        -1,24,25,26,27,28,29,30,31,-1,
        -1,32,33,34,35,36,37,38,39,-1,
        -1,40,41,42,43,44,45,46,47,-1,
        -1,48,49,50,51,52,53,54,55,-1,
        -1,56,57,58,59,60,61,62,63,-1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int mailbox64[64] = {
        21,22,23,24,25,26,27,28,
        31,32,33,34,35,36,37,38,
        41,42,43,44,45,46,47,48,
        51,52,53,54,55,56,57,58,
        61,62,63,64,65,66,67,68,
        71,72,73,74,75,76,77,78,
        81,82,83,84,85,86,87,88,
        91,92,93,94,95,96,97,98
    };

int blackPiecesValue[7][64] = {
    {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
    },
    {
    0,0,0,0,0,0,0,0,
    -100,-100,-100,-100,-100,-100,-100,-100,
    -110,-110,-80,-120,-120,-80,-110,-110,
    -110,-100,-90,-150,-150,-90,-100,-110,
    -100,-100,-100,-155,-155,-100,-100,-100,
    -130,-130,-130,-160,-160,-130,-130,-130,
    -180,-180,-180,-190,-190,-180,-180,-180,
    0,0,0,0,0,0,0,0
},

{
    -250,-280,-300,-300,-300,-300,-280,-250,
    -280,-300,-310,-320,-320,-310,-300,-280,
    -280,-310,-320,-320,-320,-320,-310,-280,
    -300,-310,-320,-330,-330,-320,-310,-300,
    -300,-300,-300,-330,-330,-300,-300,-300,
    -280,-300,-300,-300,-300,-300,-300,-280,
    -280,-300,-300,-300,-300,-300,-300,-280,
    -250,-280,-300,-300,-300,-300,-280,-250,
},

{
    -270,-280,-290,-290,-290,-290,-280,-270,
    -280,-320,-300,-320,-320,-300,-320,-280,
    -290,-320,-320,-320,-320,-320,-320,-290,
    -290,-300,-330,-300,-300,-330,-300,-290,
    -290,-300,-300,-300,-300,-300,-300,-290,
    -290,-300,-300,-300,-300,-300,-300,-290,
    -280,-300,-300,-300,-300,-300,-300,-280,
    -270,-280,-290,-290,-290,-290,-280,-270
},

{
    -500,-500,-500,-520,-520,-500,-500,-500,
    -500,-500,-500,-520,-520,-500,-500,-500,
    -500,-500,-500,-500,-500,-500,-500,-500,
    -500,-500,-500,-500,-500,-500,-500,-500,
    -500,-500,-500,-500,-500,-500,-500,-500,
    -500,-500,-500,-500,-500,-500,-500,-500,
    -530,-530,-530,-530,-530,-530,-530,-530,
    -500,-500,-500,-500,-500,-500,-500,-500
},

{
    -870,-880,-910,-920,-920,-910,-880,-870,
    -880,-910,-910,-920,-920,-910,-910,-880,
    -900,-910,-910,-920,-920,-910,-910,-900,
    -900,-910,-910,-920,-920,-910,-910,-900,
    -900,-910,-910,-920,-920,-910,-910,-900,
    -900,-910,-910,-920,-920,-910,-910,-900,
    -880,-910,-910,-920,-920,-910,-910,-880,
    -870,-880,-910,-920,-920,-910,-880,-870
},

{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
}
};

int whitePiecesValue[7][64] = {
    {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
    },
    {
    0,0,0,0,0,0,0,0,
    180,180,180,190,190,180,180,180,
    130,130,130,160,160,130,130,130,
    100,100,100,155,155,100,100,100,
    110,100,90,150,150,90,100,110,
    110,110,80,120,120,80,110,110,
    100,100,100,100,100,100,100,100,
    0,0,0,0,0,0,0,0
},

{
    250,280,300,300,300,300,280,250,
    280,300,300,300,300,300,300,280,
    280,300,300,300,300,300,300,280,
    300,300,300,330,330,300,300,300,
    300,310,320,330,330,320,310,300,
    280,310,320,320,320,320,310,280,
    280,300,310,320,320,310,300,280,
    250,280,300,300,300,300,280,250
},

{
    270,280,290,290,290,290,280,270,
    280,300,300,300,300,300,300,280,
    290,300,300,300,300,300,300,290,
    290,300,300,300,300,300,300,290,
    290,300,330,300,300,330,300,290,
    290,320,320,320,320,320,320,290,
    280,320,300,320,320,300,320,280,
    270,280,290,290,290,290,280,270
},

{
    500,500,500,500,500,500,500,500,
    530,530,530,530,530,530,530,530,
    500,500,500,500,500,500,500,500,
    500,500,500,500,500,500,500,500,
    500,500,500,500,500,500,500,500,
    500,500,500,500,500,500,500,500,
    500,500,500,520,520,500,500,500,
    500,500,500,520,520,500,500,500
},

{
    870,880,910,920,920,910,880,870,
    880,910,910,920,920,910,910,880,
    900,910,910,920,920,910,910,900,
    900,910,910,920,920,910,910,900,
    900,910,910,920,920,910,910,900,
    900,910,910,920,920,910,910,900,
    880,910,910,920,920,910,910,880,
    870,880,910,920,920,910,880,870
},

{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
}
};

int pieceValues[7] = {0,1,3,3,5,9,100};

struct chessPosition{
    int color[64];
    int piece[64];
    bool castling[4];
    int fiftyMove;
    int toMove;
    int enPassantTargetSquare;
    int depth;
    int priority;
    int fromSquare;
    int toSquare;
    int promotionPiece;
    chessPosition(){
        
        int colorStart[64] = {
            -1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1
        };

        /*
        0 empty
        1 pawn
        2 knight
        3 bishop
        4 rook
        5 queen
        6 king
        */
       
        int pieceStart[64] = {
            4,2,3,5,6,3,2,4,
            1,1,1,1,1,1,1,1,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,
            1,1,1,1,1,1,1,1,
            4,2,3,5,6,3,2,4
        };
        
        int castlingStart[4] = {1,1,1,1};
        std::copy(colorStart, colorStart+64, color);
        std::copy(pieceStart, pieceStart+64, piece);
        std::copy(castlingStart, castlingStart+4, castling);
        fiftyMove = 0;
        toMove = 1;
        enPassantTargetSquare = -1;
        depth = 0;
    }
    chessPosition(chessPosition* cp){
        std::copy(cp->color, cp->color+64, color);
        std::copy(cp->piece, cp->piece+64, piece);
        std::copy(cp->castling, cp->castling+4, castling);
        fiftyMove = cp->fiftyMove;
        toMove = cp->toMove;
        enPassantTargetSquare = cp->enPassantTargetSquare;
        depth = cp->depth;
    }
};

struct resultingPositions{
    //std::vector<chessPosition> nonCaptures;
    //std::vector<chessPosition> captures;
    //0 not valid
    //1 valid
    int eval;
    int fromSquare;
    int toSquare;
    int promotionPiece = 0;
    int enPassantTargetSquare = -1;
    int priority;
    //0 absolute
    //-1 less than/equal to
    //1 greater than/equal to
    int boundType;
};

struct move{
    int fromSquare;
    int toSquare;
    int promotionPiece = 0;
    bool isEnPassant = false;
    bool isCastling = false;
    bool isPromotion = false;
    int capturedPiece = 0;
    int priority;
};

struct hashTableStore{
    unsigned long long int hashKey;
    resultingPositions moveToPlay;
    int depth;
    bool valid;
};

int evaluatePosition(chessPosition*);

resultingPositions evaluatePositionToQuiet(chessPosition*, int, resultingPositions);

bool checkIfSquareInCheck(chessPosition*, int);

bool kingInDanger(chessPosition*);

bool priorityCompare (move*, move*);

void afterPosition(chessPosition*, resultingPositions);

void afterMove(chessPosition*, move*);

void reverseMove(chessPosition*, move*);

int staticExchangeEvaluation(chessPosition*, move*);

//int staticExchangeEvaluationContinue(chessPosition*, int);

int staticExchangeEvaluationContinue(std::array<std::vector<int>,18>*, int, int);

std::array<std::vector<int>,18> getAllAttackers(chessPosition* cp, int square);

//std::array<std::vector<int>,18> getSmallestAttacker(chessPosition* cp, int square);

int numPositions = 0;

hashTableStore hashTable[HASH_TABLE_SIZE];

void resetHashTable(){
    //printf("resetting\n");
    for(int i=0; i<HASH_TABLE_SIZE; i++){
        hashTable[i].valid = false;
    }
}

void generateRandom(){
    std::mt19937_64 randomGenerator (RANDOM_SEED);
    for(int i=0; i<781; i++){
        bitSequences[i] = randomGenerator();
    }
}

unsigned long long int hashKey(chessPosition* cp){
    unsigned long long int hash = 0;
    std::bit_xor<unsigned long long int> xorer;
    for(int i=0; i<64; i++){
        if(cp->color[i] != 0){
            int a = cp->color[i]*cp->piece[i];
            if(a>0){
                a--;
            }
            int b = (a+6)*64+i;
            hash = xorer(hash, bitSequences[b]);
        }
    }
    if(cp->toMove == -1){
        hash = xorer(hash, bitSequences[768]);
    }
    for(int i=0; i<4; i++){
        if(cp->castling[i] == 1){
            hash = xorer(hash, bitSequences[769+i]);
        }
    }
    if(cp->enPassantTargetSquare != -1){
        hash = xorer(hash, bitSequences[773+(cp->enPassantTargetSquare % 8)]);
    }
    //printf("%d\n", hash);
    return hash;
}

resultingPositions generateMoves(chessPosition* cp, int bound, bool lastNullMove, resultingPositions killerRp){
    
    /*
    if(cp->depth == 2 && cp->color[6] == 0 && cp->color[21] == -1 && cp->color[35] == 0 && cp->color[27] == 1){
        printf("here2\n");
    }
    */
    
    int toMove = cp->toMove;
    resultingPositions rp;

    unsigned long long int hash = hashKey(cp);
    int hashMod = hash % HASH_TABLE_SIZE;
    
    /*
    if(hash == 15647290675024346069){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                printf("%d ",cp->piece[8*i+j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    */
    

    if(hashTable[hashMod].valid && hashTable[hashMod].hashKey == hash && hashTable[hashMod].depth <= cp->depth){
        //printf("collision %d %d\n", hashTable[hashMod].depth, cp->depth);
        
        if(hashTable[hashMod].moveToPlay.boundType == 0){
            //absolute eval
            //printf("absolute\n");
            rp = hashTable[hashMod].moveToPlay;
            return rp;
        }
        
        if(toMove == 1 && hashTable[hashMod].moveToPlay.eval >= bound && hashTable[hashMod].moveToPlay.boundType == 1){
            //Black shouldn't consider the parent move
            //printf("bound1\n");
            rp.eval = hashTable[hashMod].moveToPlay.eval;
            rp.boundType = hashTable[hashMod].moveToPlay.boundType;
            return rp;
        }
        if(toMove == -1 && hashTable[hashMod].moveToPlay.eval <= bound && hashTable[hashMod].moveToPlay.boundType == -1){
            //White shouldn't consider the parent move
            //printf("bound2\n");
            rp.eval = hashTable[hashMod].moveToPlay.eval;
            rp.boundType = hashTable[hashMod].moveToPlay.boundType;
            return rp;
        }
        
    }

    if(cp->depth >= maxDepth){
        resultingPositions dummy;
        rp.eval = evaluatePositionToQuiet(cp, rp.eval, dummy).eval;
        return rp;
        //printf("%d\n",rp.eval);
    }

    if(!lastNullMove && cp->depth <= NULL_MOVE_DEPTH){
        cp->toMove = -cp->toMove;
        cp->depth += 1;
        resultingPositions dummy;
        int eval = generateMoves(cp, bound, true, dummy).eval;
        cp->depth -= 1;
        cp->toMove = -cp->toMove;
        if(cp->toMove == 1 && eval > bound || cp->toMove == -1 && eval < bound){
            rp.eval = eval;
            rp.boundType = toMove;
            hashTable[hashMod].valid = true;
            hashTable[hashMod].moveToPlay = rp;
            hashTable[hashMod].hashKey = hash;
            hashTable[hashMod].depth = cp->depth;
            return rp;
        }
    }
    
    //numPositions++;
    //printf("%d\n", numPositions);
    int legalMoves = 0;
    //default
    rp.boundType = 0;
    std::vector<move*> moves;
    int oldFiftyMove = cp->fiftyMove;
    int oldEnPassantTargetSquare = cp->enPassantTargetSquare;
    int castling[4];
    std::copy(cp->castling,cp->castling+4,castling);
    if(toMove == 1){
        rp.eval = -99999;
    }
    else{
        rp.eval = 99999;
    }
    if(cp->fiftyMove >= 50){
        //printf("here\n");
        rp.eval = 0;
        return rp;
    }
    //castling
    if(toMove == 1){
        if(cp->castling[0] == 1){
            if(cp->color[60] == 1 && cp->piece[60] == 6 && cp->color[56] == 1 && cp->piece[56] == 4){
                if(cp->color[57] == 0 && cp->color[58] == 0 && cp->color[59] == 0 && !checkIfSquareInCheck(cp,59) && !checkIfSquareInCheck(cp,60)){
                        move* m = new move();
                        m->fromSquare = 60;
                        m->toSquare = 58;
                        m->priority = QUIET_MOVE_PRIORITY;
                        m->isCastling = true;
                        moves.push_back(m);
                }
            }
        }
        if(cp->castling[1] == 1){
            if(cp->color[60] == 1 && cp->piece[60] == 6 && cp->color[63] == 1 && cp->piece[63] == 4){
                //printf("here\n");
                if(cp->color[61] == 0 && cp->color[62] == 0 && !checkIfSquareInCheck(cp,60) && !checkIfSquareInCheck(cp,61)){
                    //printf("here2\n");
                        //printf("here3\n");
                        //numPositions++;
                        //printf("%d\n", numPositions);
                        move* m = new move();
                        m->fromSquare = 60;
                        m->toSquare = 62;
                        m->priority = QUIET_MOVE_PRIORITY;
                        m->isCastling = true;
                        moves.push_back(m);
                }
            }
        }
    }
    else{
        if(cp->castling[2] == 1){
            if(cp->color[4] == -1 && cp->piece[4] == 6 && cp->color[0] == -1 && cp->piece[0] == 4){
                if(cp->color[1] == 0 && cp->color[2] == 0 && cp->color[3] == 0 && !checkIfSquareInCheck(cp,3) && !checkIfSquareInCheck(cp,4)){
                        move* m = new move();
                        m->fromSquare = 4;
                        m->toSquare = 2;
                        m->priority = QUIET_MOVE_PRIORITY;
                        m->isCastling = true;
                        moves.push_back(m);
                }
            }
        }
        if(cp->castling[3] == 1){
            if(cp->color[4] == -1 && cp->piece[4] == 6 && cp->color[7] == -1 && cp->piece[7] == 4){
                if(cp->color[5] == 0 && cp->color[6] == 0 && !checkIfSquareInCheck(cp,4) && !checkIfSquareInCheck(cp,5)){
                        move* m = new move();
                        m->fromSquare = 4;
                        m->toSquare = 6;
                        m->priority = QUIET_MOVE_PRIORITY;
                        m->isCastling = true;
                        moves.push_back(m);
                }
            }
        }
    }
    int pieceType;
    for(int i=0; i<64; i++){
        if(cp->color[i]==toMove){
            pieceType = cp->piece[i];
            if(pieceType!=1){
                //not a pawn
                for(int j=0; j<numDirections[pieceType]; j++){
                    for(int destination = i;;){
                        destination = mailbox[mailbox64[destination] + offsets[pieceType][j]];
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0){
                            if(cp->color[destination] != toMove){
                                if(cp->piece[destination] == 6){
                                    /*
                                    //King capture. Invalid. Should never happen now
                                    rp.valid = 0;
                                    for(int j=0; j<moves.size(); j++){
                                        delete(moves[j]);
                                    }
                                    return rp;
                                    */
                                }
                                //capture
                                //TODO capture
                                    move* m = new move();
                                    m->fromSquare = i;
                                    m->toSquare = destination;
                                    //m->priority = 8+pieceValues[cp->piece[i]]-pieceValues[cp->piece[destination]];
                                    m->capturedPiece = cp->piece[destination];
                                    m->priority = 50 - staticExchangeEvaluation(cp, m);
                                    cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                    moves.push_back(m);
                            }
                            break;
                        }
                        //move to an empty square
                        //TODO move to an empty square
                                    move* m = new move();
                                    m->fromSquare = i;
                                    m->toSquare = destination;
                                    if(cp->depth <= QUIET_STATIC_DEPTH){
                                        m->priority = QUIET_MOVE_PRIORITY - staticExchangeEvaluation(cp, m);
                                    }
                                    else{
                                        m->priority = QUIET_MOVE_PRIORITY;
                                    }
                                    cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                    moves.push_back(m);
                        if(!slide[pieceType]){
                            break;
                        }
                    }
                }
            }
            else{
                //a pawn
                //TODO
                    bool frontClear = cp->piece[i-8*toMove] == 0;
                    if(frontClear){
                            if((i/8 == 1 && toMove == -1) || (i/8 == 6 && toMove == 1)){
                                bool frontClear2 = cp->piece[i-16*toMove] == 0;
                                if(frontClear2){
                                        move* m = new move();
                                        m->fromSquare = i;
                                        m->toSquare = i-16*toMove;
                                        if(cp->depth <= QUIET_STATIC_DEPTH){
                                            m->priority = QUIET_MOVE_PRIORITY - staticExchangeEvaluation(cp, m);
                                        }
                                        else{
                                            m->priority = QUIET_MOVE_PRIORITY;
                                        }
                                        cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                        moves.push_back(m);
                                }
                            }
                                if((i/8 == 6 && toMove == -1) || (i/8 == 1 && toMove == 1)){
                                    for(int j=2; j<6; j++){
                                            move* m = new move();
                                            m->fromSquare = i;
                                            m->toSquare = i-8*toMove;
                                            m->promotionPiece = j;
                                            m->isPromotion = true;
                                            if(cp->depth <= QUIET_STATIC_DEPTH){
                                                m->priority = QUIET_MOVE_PRIORITY - pieceValues[j] - staticExchangeEvaluation(cp, m);
                                            }
                                            else{
                                                m->priority = QUIET_MOVE_PRIORITY;
                                            }
                                            moves.push_back(m);
                                    }
                                }
                                else{
                                    move* m = new move();
                                    m->fromSquare = i;
                                    m->toSquare = i-8*toMove;
                                    if(cp->depth <= QUIET_STATIC_DEPTH){
                                        m->priority = QUIET_MOVE_PRIORITY - staticExchangeEvaluation(cp, m);
                                    }
                                    else{
                                        m->priority = QUIET_MOVE_PRIORITY;
                                    }
                                    cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                    moves.push_back(m);
                                }
                    }
                    //pawn captures
                    int captureDestination1 = mailbox[mailbox64[i] - 9*toMove];
                    int captureDestination2 = mailbox[mailbox64[i] - 11*toMove];
                        if(captureDestination1 != -1){
                            bool isEnPassant = (cp->enPassantTargetSquare == captureDestination1) && !lastNullMove;
                                if(cp->color[captureDestination1] == -toMove || isEnPassant){
                                    /*
                                    //King capture should never happen now
                                    if(cp->piece[captureDestination1] == 6){
                                        rp.valid = 0;
                                        for(int j=0; j<moves.size(); j++){
                                            delete(moves[j]);
                                        }
                                        return rp;
                                    }
                                    */
                                    if((i/8 == 6 && toMove == -1) || (i/8 == 1 && toMove == 1)){
                                        for(int j=2; j<6; j++){
                                                move* m = new move();
                                                m->fromSquare = i;
                                                m->toSquare = captureDestination1;
                                                m->capturedPiece = cp->piece[captureDestination1];
                                                //m->priority = 9-pieceValues[cp->piece[captureDestination1]];
                                                m->promotionPiece = j;
                                                m->isPromotion = true;
                                                m->priority = 50 - pieceValues[j] - staticExchangeEvaluation(cp, m);
                                                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                                moves.push_back(m);
                                        }
                                    }
                                    else{
                                        move* m = new move();
                                        m->fromSquare = i;
                                        m->toSquare = captureDestination1;
                                        m->capturedPiece = cp->piece[captureDestination1];
                                        //m->priority = 9-pieceValues[cp->piece[captureDestination1]];
                                        if(isEnPassant){
                                            m->isEnPassant = true;
                                        }
                                        m->priority = 50 - staticExchangeEvaluation(cp, m);
                                        cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                        moves.push_back(m);
                                    }
                                }
                        }
                        if(captureDestination2 != -1){
                            bool isEnPassant = (cp->enPassantTargetSquare == captureDestination2) && !lastNullMove;
                                if(cp->color[captureDestination2] == -toMove || isEnPassant){
                                    /*
                                    //King capture should never happen now
                                    if(cp->piece[captureDestination2] == 6){
                                        rp.valid = 0;
                                        for(int j=0; j<moves.size(); j++){
                                            delete(moves[j]);
                                        }
                                        return rp;
                                    }
                                    */
                                    if((i/8 == 6 && toMove == -1) || (i/8 == 1 && toMove == 1)){
                                        for(int j=2; j<6; j++){
                                                move* m = new move();
                                                m->fromSquare = i;
                                                m->toSquare = captureDestination2;
                                                m->capturedPiece = cp->piece[captureDestination2];
                                                //m->priority = 9-pieceValues[cp->piece[captureDestination2]];
                                                m->promotionPiece = j;
                                                m->isPromotion = true;
                                                m->priority = 50 - pieceValues[j] - staticExchangeEvaluation(cp, m);
                                                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                                moves.push_back(m);
                                        }
                                    }
                                    else{
                                        move* m = new move();
                                        m->fromSquare = i;
                                        m->toSquare = captureDestination2;
                                        m->capturedPiece = cp->piece[captureDestination2];
                                        //m->priority = 9-pieceValues[cp->piece[captureDestination2]];
                                        if(isEnPassant){
                                            m->isEnPassant = true;
                                        }
                                        m->priority = 50 - staticExchangeEvaluation(cp, m);
                                        cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                        moves.push_back(m);
                                    }
                                }
                        }
            }
        }
    }

    //if(rp.captures.size() == 0 && rp.nonCaptures.size() == 0)
    
    //printf("%d\n",moves.size());

    //iterative deepening I guess
    
    cp->depth += 2;
    resultingPositions iterative = generateMoves(cp,bound,lastNullMove,killerRp);
    int evalFromIterative = iterative.eval;
    cp->depth -= 2;
    int trueBound = bound;
    if(toMove == -1){
        //Black to move. Set guess for bound at previous eval minus 0.25
        bound = std::max(trueBound,evalFromIterative-25);
    }
    else{
        //White to move. Set guess for bound at previous eval plus 0.25
        bound = std::min(trueBound,evalFromIterative+25);
    }
    int numWindowFails = 0;

    for(int i=0; i<moves.size(); i++){
        
        if(moves[i]->fromSquare == iterative.fromSquare && moves[i]->toSquare == iterative.toSquare && moves[i]->promotionPiece == iterative.promotionPiece){
            moves[i]->priority = ITERATIVE_BEST_PRIORITY;
        }
        
        if(moves[i]->fromSquare == killerRp.fromSquare && moves[i]->toSquare == killerRp.toSquare && moves[i]->promotionPiece == killerRp.promotionPiece){
            moves[i]->priority -= KILLER_PRIORITY_BONUS;
        }
    }

    std::sort(moves.begin(),moves.end(),priorityCompare);

    //printf("%d\n",moves.size());
    for(int i=0; i<moves.size(); i++){
        resultingPositions killerRp;
        afterMove(cp, moves[i]);
        cp->toMove = -cp->toMove;
        if(!kingInDanger(cp)){
            cp->toMove = -cp->toMove;
            cp->depth += 1;
            resultingPositions newRp;

            clock_t timer;
            timer = clock();

/*
            bool reduce = moves[i]->priority > QUIET_MOVE_PRIORITY;
            if(reduce){
                maxDepth -= 1;
            }
            */
            newRp = generateMoves(cp, rp.eval, false, killerRp);
            /*
            if(reduce){
                maxDepth += 1;
            }
            */
            killerRp = newRp;
            
            if(cp->depth == 1 && printTimeStats){
                printf("%c%d %c%d %.2f %.2fs\n",moves[i]->fromSquare % 8 + 97,8-moves[i]->fromSquare / 8,moves[i]->toSquare % 8 + 97,8-moves[i]->toSquare / 8, ((float)newRp.eval)/100, (float)(clock()-timer)/CLOCKS_PER_SEC);
            }
            /*
            if(cp->depth == 3 && cp->color[6] == 0 && cp->color[21] == -1 && cp->color[35] == 0 && cp->color[27] == 1){
                printf("%d %d %d\n", moves[i]->fromSquare, moves[i]->toSquare, newRp.eval);
            }
            */
            
            cp->depth -= 1;
            reverseMove(cp, moves[i]);
            cp->fiftyMove = oldFiftyMove;
            cp->enPassantTargetSquare = oldEnPassantTargetSquare;
            std::copy(castling,castling+4, cp->castling);
            //rp.captures.push_back(newCp);
            legalMoves++;
            if((toMove == 1 && newRp.eval >= bound)||(toMove == -1 && newRp.eval <= bound)){
                if(bound != trueBound){
                    //failed with aspiration window, search again
                    numWindowFails++;
                    if(toMove == -1){
                        bound = std::max(trueBound, evalFromIterative-25*(int)round(exp2(numWindowFails)));
                    }
                    else{
                        bound = std::min(trueBound, evalFromIterative+25*(int)round(exp2(numWindowFails)));
                    }
                    i--;
                    continue;
                }
                rp.eval = newRp.eval;
                rp.boundType = toMove;
                rp.fromSquare = moves[i]->fromSquare;
                rp.toSquare = moves[i]->toSquare;
                rp.promotionPiece = moves[i]->promotionPiece;
                for(int j=0; j<moves.size(); j++){
                    delete(moves[j]);
                }
                hashTable[hashMod].valid = true;
                hashTable[hashMod].moveToPlay = rp;
                hashTable[hashMod].hashKey = hash;
                hashTable[hashMod].depth = cp->depth;
                return rp;
            }
            if(toMove == 1 && newRp.eval > rp.eval){
                rp.eval = newRp.eval;
                    rp.fromSquare = moves[i]->fromSquare;
                    rp.toSquare = moves[i]->toSquare;
                    rp.promotionPiece = moves[i]->promotionPiece;
                    //rp.resultingPosition = newCp;
            }
            if(toMove == -1 && newRp.eval < rp.eval){
                rp.eval = newRp.eval;
                    rp.fromSquare = moves[i]->fromSquare;
                    rp.toSquare = moves[i]->toSquare;
                    rp.promotionPiece = moves[i]->promotionPiece;
                    //rp.resultingPosition = newCp;
            }
        }
        else{
                cp->toMove = -cp->toMove;
                reverseMove(cp, moves[i]);
                cp->fiftyMove = oldFiftyMove;
                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                std::copy(castling,castling+4, cp->castling);
            }
    }

    if(legalMoves == 0){
        //printf("what happened?\n");
        //no legal moves
        //check if stalemate or checkmate
        bool isCheckmate = false;
        for(int i=0; i<64; i++){
            if(cp->color[i] == toMove && cp->piece[i] == 6){
                isCheckmate = checkIfSquareInCheck(cp, i);
                break;
            }
        }
        if(isCheckmate){
            //checkmate
            rp.eval = -(99998-cp->depth)*toMove;
        }
        else{
            //stalemate
            rp.eval = 0;
        }
    }
    for(int j=0; j<moves.size(); j++){
        delete(moves[j]);
    }
    hashTable[hashMod].valid = true;
    hashTable[hashMod].moveToPlay = rp;
    hashTable[hashMod].hashKey = hash;
    hashTable[hashMod].depth = cp->depth;
    return rp;
}

bool priorityCompare (move* cp1, move* cp2){
    return cp1->priority < cp2->priority;
}

int evaluatePosition(chessPosition* cp){
    int total = 0;
    for(int i=0; i<64; i++){
        if(cp->color[i] == -1){
            total += blackPiecesValue[cp->piece[i]][i];
        }
        else if(cp->color[i] == 1){
            total += whitePiecesValue[cp->piece[i]][i];
        }
    }
    return total;
}

resultingPositions evaluatePositionToQuiet(chessPosition* cp, int bound, resultingPositions killerRp){
    resultingPositions rp;
    bool captureExists = false;
    std::vector<move*> moves;
    int toMove = cp->toMove;

/*
    unsigned long long int hash = hashKey(cp);
    int hashMod = hash % HASH_TABLE_SIZE;
    
    //comment from
    if(hash == 15647290675024346069){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                printf("%d ",cp->piece[8*i+j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    //comment to
    

    if(hashTable[hashMod].valid && hashTable[hashMod].hashKey == hash && hashTable[hashMod].depth <= cp->depth){
        //printf("collision %d %d\n", hashTable[hashMod].depth, cp->depth);
        
        if(hashTable[hashMod].moveToPlay.boundType == 0){
            //absolute eval
            //printf("absolute\n");
            rp = hashTable[hashMod].moveToPlay;
            return rp;
        }
        
        if(toMove == 1 && hashTable[hashMod].moveToPlay.eval >= bound && hashTable[hashMod].moveToPlay.boundType == 1){
            //Black shouldn't consider the parent move
            //printf("bound1\n");
            rp.eval = hashTable[hashMod].moveToPlay.eval;
            rp.boundType = hashTable[hashMod].moveToPlay.boundType;
            return rp;
        }
        if(toMove == -1 && hashTable[hashMod].moveToPlay.eval <= bound && hashTable[hashMod].moveToPlay.boundType == -1){
            //White shouldn't consider the parent move
            //printf("bound2\n");
            rp.eval = hashTable[hashMod].moveToPlay.eval;
            rp.boundType = hashTable[hashMod].moveToPlay.boundType;
            return rp;
        }
        
    }
    */

    int oldFiftyMove = cp->fiftyMove;
    int oldEnPassantTargetSquare = cp->enPassantTargetSquare;
    int castling[4];
    std::copy(cp->castling,cp->castling+4,castling);
    if(toMove == -1){
        rp.eval = 99999;
    }
    else{
        rp.eval = -99999;
    }
    int pieceType;
    for(int i=0; i<64; i++){
        if(cp->color[i]==toMove){
            pieceType = cp->piece[i];
            if(pieceType!=1){
                //not a pawn
                for(int j=0; j<numDirections[pieceType]; j++){
                    for(int destination = i;;){
                        destination = mailbox[mailbox64[destination] + offsets[pieceType][j]];
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0 && cp->piece[destination] != 6){
                            if(cp->color[destination] != toMove){
                                //capture
                                //TODO capture
                                    move* m = new move();
                                    m->fromSquare = i;
                                    m->toSquare = destination;
                                    m->capturedPiece = cp->piece[destination];
                                    m->priority = 50 - staticExchangeEvaluation(cp, m);
                                    cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                    if(m->priority <= 50){
                                        moves.push_back(m);
                                    }
                                    else{
                                        delete(m);
                                    }
                            }
                            break;
                        }
                        if(!slide[pieceType]){
                            break;
                        }
                    }
                }
            }
            else{
                //a pawn
                //TODO
                    //pawn captures
                    int captureDestination1 = mailbox[mailbox64[i] - 9*toMove];
                    int captureDestination2 = mailbox[mailbox64[i] - 11*toMove];
                        if(captureDestination1 != -1){
                            bool isEnPassant = cp->enPassantTargetSquare == captureDestination1;
                                if(cp->color[captureDestination1] == -toMove || isEnPassant){
                                    if((i/8 == 6 && toMove == -1) || (i/8 == 1 && toMove == 1)){
                                        for(int j=2; j<6; j++){
                                                move* m = new move();
                                                m->fromSquare = i;
                                                m->toSquare = captureDestination1;
                                                m->capturedPiece = cp->piece[captureDestination1];
                                                m->promotionPiece = j;
                                                m->isPromotion = true;
                                                m->priority = 50 - staticExchangeEvaluation(cp, m) - pieceValues[j];
                                                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                                if(m->priority <= 50){
                                                    moves.push_back(m);
                                                }
                                                else{
                                                    delete(m);
                                                }
                                        }
                                    }
                                    else{
                                        move* m = new move();
                                        m->fromSquare = i;
                                        m->toSquare = captureDestination1;
                                        m->capturedPiece = cp->piece[captureDestination1];
                                        if(isEnPassant){
                                            m->isEnPassant = true;
                                        }
                                        m->priority = 50 - staticExchangeEvaluation(cp, m);
                                        cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                        if(m->priority <= 50){
                                            moves.push_back(m);
                                        }
                                        else{
                                            delete(m);
                                        }
                                    }
                                }
                        }
                        if(captureDestination2 != -1){
                            bool isEnPassant = cp->enPassantTargetSquare == captureDestination2;
                                if(cp->color[captureDestination2] == -toMove || isEnPassant){
                                    if((i/8 == 6 && toMove == -1) || (i/8 == 1 && toMove == 1)){
                                        for(int j=2; j<6; j++){
                                                move* m = new move();
                                                m->fromSquare = i;
                                                m->toSquare = captureDestination2;
                                                m->capturedPiece = cp->piece[captureDestination2];
                                                m->promotionPiece = j;
                                                m->isPromotion = true;
                                                m->priority = 50 - staticExchangeEvaluation(cp, m) - pieceValues[j];
                                                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                                if(m->priority <= 50){
                                                    moves.push_back(m);
                                                }
                                                else{
                                                    delete(m);
                                                }
                                        }
                                    }
                                    else{
                                        move* m = new move();
                                        m->fromSquare = i;
                                        m->toSquare = captureDestination2;
                                        m->capturedPiece = cp->piece[captureDestination2];
                                        if(isEnPassant){
                                            m->isEnPassant = true;
                                        }
                                        m->priority = 50 - staticExchangeEvaluation(cp, m);
                                        cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                                        if(m->priority <= 50){
                                            moves.push_back(m);
                                        }
                                        else{
                                            delete(m);
                                        }
                                    }
                                }
                        }
            }
        }
    }
    bool legalCaptureExists = false;
    if(moves.size() == 0){
        rp.eval = evaluatePosition(cp);
        /*
        hashTable[hashMod].valid = true;
        hashTable[hashMod].moveToPlay = rp;
        hashTable[hashMod].hashKey = hash;
        hashTable[hashMod].depth = cp->depth;
        */
        return rp;
    }
    else{
        for(int i=0; i<moves.size(); i++){
            if(moves[i]->fromSquare == killerRp.fromSquare && moves[i]->toSquare == killerRp.toSquare && moves[i]->promotionPiece == killerRp.promotionPiece){
                moves[i]->priority -= KILLER_PRIORITY_BONUS_QUIESCENCE;
            }
        }

        std::sort(moves.begin(),moves.end(),priorityCompare);

        for(int i=0; i<moves.size(); i++){
            resultingPositions killerRp;
            afterMove(cp, moves[i]);
            cp->toMove = -cp->toMove;
            if(!kingInDanger(cp)){
                legalCaptureExists = true;
                cp->toMove = -cp->toMove;
                resultingPositions newRp = evaluatePositionToQuiet(cp, rp.eval, killerRp);
                killerRp = newRp;
                reverseMove(cp, moves[i]);
                cp->fiftyMove = oldFiftyMove;
                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                std::copy(castling,castling+4, cp->castling);
                    //rp.captures.push_back(newCp);
                    if((cp->toMove == 1 && newRp.eval >= bound)||(cp->toMove == -1 && newRp.eval <= bound)){
                        rp.eval = newRp.eval;
                        rp.fromSquare = moves[i]->fromSquare;
                        rp.toSquare = moves[i]->toSquare;
                        rp.promotionPiece = moves[i]->promotionPiece;
                        for(int j=0; j<moves.size(); j++){
                            delete(moves[j]);
                        }
                        /*
                        hashTable[hashMod].valid = true;
                        hashTable[hashMod].moveToPlay = rp;
                        hashTable[hashMod].hashKey = hash;
                        hashTable[hashMod].depth = cp->depth;
                        */
                        return rp;
                    }
                    if(cp->toMove == 1 && newRp.eval > rp.eval){
                        rp.fromSquare = moves[i]->fromSquare;
                        rp.toSquare = moves[i]->toSquare;
                        rp.promotionPiece = moves[i]->promotionPiece;
                        rp.eval = newRp.eval;
                    }
                    if(cp->toMove == -1 && newRp.eval < rp.eval){
                        rp.fromSquare = moves[i]->fromSquare;
                        rp.toSquare = moves[i]->toSquare;
                        rp.promotionPiece = moves[i]->promotionPiece;
                        rp.eval = newRp.eval;
                    }
            }
            else{
                cp->toMove = -cp->toMove;
                reverseMove(cp, moves[i]);
                cp->fiftyMove = oldFiftyMove;
                cp->enPassantTargetSquare = oldEnPassantTargetSquare;
                std::copy(castling,castling+4, cp->castling);
            }
        }
    }
    for(int j=0; j<moves.size(); j++){
        delete(moves[j]);
    }

    if(!legalCaptureExists){
        rp.eval = evaluatePosition(cp);
        /*
        hashTable[hashMod].valid = true;
        hashTable[hashMod].moveToPlay = rp;
        hashTable[hashMod].hashKey = hash;
        hashTable[hashMod].depth = cp->depth;
        */
        return rp;
    }

    int doNothingEval = evaluatePosition(cp);
    //doing nothing could be better than capturing
    if((cp->toMove == 1 && doNothingEval >= bound)||(cp->toMove == -1 && doNothingEval <= bound)){
        rp.eval = doNothingEval;
        for(int j=0; j<moves.size(); j++){
            delete(moves[j]);
        }
        /*
        hashTable[hashMod].valid = true;
        hashTable[hashMod].moveToPlay = rp;
        hashTable[hashMod].hashKey = hash;
        hashTable[hashMod].depth = cp->depth;
        */
        return rp;
    }
    if(cp->toMove == 1 && doNothingEval > rp.eval){
        rp.eval = doNothingEval;
    }
    if(cp->toMove == -1 && doNothingEval < rp.eval){
        rp.eval = doNothingEval;
    }
    
    /*
    if(rp.eval==-99999){
        printf("oops\n");
    }
    */
    
    /*
    hashTable[hashMod].valid = true;
    hashTable[hashMod].moveToPlay = rp;
    hashTable[hashMod].hashKey = hash;
    hashTable[hashMod].depth = cp->depth;
    */
    return rp;
}

int staticExchangeEvaluationContinue(std::array<std::vector<int>,18> attackers, int toMove, int captureValue){
    int index;
    int smallestAttacker = 999;
    if(toMove == -1){
        for(int i=0; i<9; i++){
            if(attackers[i].size() > 0){
                if(attackers[i][0] < smallestAttacker){
                    index = i;
                    smallestAttacker = attackers[i][0];
                }
            }
        }
    }
    if(toMove == 1){
        for(int i=9; i<18; i++){
            if(attackers[i].size() > 0){
                if(attackers[i][0] < smallestAttacker){
                    index = i;
                    smallestAttacker = attackers[i][0];
                }
            }
        }
    }
    if(smallestAttacker != 999){
        attackers[index].erase(attackers[index].begin());
        return std::max(0,captureValue - staticExchangeEvaluationContinue(attackers,-toMove,smallestAttacker));
    }
    else{
        return 0;
    }
}

/*
int staticExchangeEvaluationContinue(chessPosition* cp, int square){
    //comment from
    if(cp->toMove == cp->color[square]){
        printf("bad\n");
    }
    //comment to
    chessPosition cp2 = *cp;
    int value = 0;
    int fromSquare = getSmallestAttacker(cp, square);
    int captureValue = pieceValues[cp->piece[square]];
    if(fromSquare != -1){
        //comment from
        if(cp->toMove != cp->color[fromSquare]){
            printf("bad\n");
        }
        //comment to
        move* m = new move();
        m->fromSquare = fromSquare;
        m->toSquare = square;
        m->capturedPiece = cp->piece[square];
        if(cp->piece[fromSquare] == 1 && ((square/8 == 7 && cp->toMove == -1) || (square/8 == 0 && cp->toMove == 1))){
            m->promotionPiece = 5;
            m->isPromotion = true;
            afterMove(cp,m);
            value = std::max(0,captureValue + 8 - staticExchangeEvaluationContinue(cp,square));
            reverseMove(cp,m);
        }
        else{
            afterMove(cp,m);
            value = std::max(0,captureValue - staticExchangeEvaluationContinue(cp,square));
            reverseMove(cp,m);
        }
        delete(m);
    }
    
    //comment from
    for(int i=0; i<64; i++){
        if(cp2.piece[i] != cp->piece[i] || cp2.color[i] != cp->color[i]){
            printf("badpiece %d\n", i);
        }
        if(cp2.color[i] != cp->color[i]){
            printf("badcolor %d\n", i);
        }
    }
    //comment to
    
    return value;
}
*/

int staticExchangeEvaluation(chessPosition* cp, move* m){

    //return -1;
    //chessPosition cp2 = *cp;
    
    //too slow so do this instead for now
    int value = 0;
    int captureValue = pieceValues[cp->piece[m->toSquare]];
    return captureValue - pieceValues[cp->piece[m->fromSquare]];
    /*
    afterMove(cp,m);
    std::array<std::vector<int>,18> attackers = getAllAttackers(cp,m->toSquare);
    value = captureValue - staticExchangeEvaluationContinue(attackers,cp->toMove,pieceValues[cp->piece[m->toSquare]]);
    reverseMove(cp,m);
    
    //comment from
    for(int i=0; i<64; i++){
        if(cp2.piece[i] != cp->piece[i] || cp2.color[i] != cp->color[i]){
            printf("badpiece %d\n", i);
        }
        if(cp2.color[i] != cp->color[i]){
            printf("badcolor %d\n", i);
        }
    }
    //comment to
    
    return value;
    */
}

std::array<std::vector<int>,18> getAllAttackers(chessPosition* cp, int square){
    std::array<std::vector<int>,18> allAttackers = std::array<std::vector<int>,18>();
    int smallestAttacker = 99999;
    int squareOfSmallestAttacker = -1;
    for(int i=0; i<8; i++){
            int destination = square;
            destination = mailbox[mailbox64[destination] + offsets[2][i]];
            if(destination == -1){
                continue;
            }
            if(cp->piece[destination] == 2){
                if(cp->color[destination] == -1){
                    allAttackers[8].push_back(3);
                    //smallestAttacker = 300;
                    //squareOfSmallestAttacker = destination;
                }
                else{
                    allAttackers[17].push_back(3);
                }
            }
        }
        //printf("here\n");
        for(int i=0; i<4; i++){
            //printf("%d\n",i);
            int spacesAway = 0;
                    for(int destination = square;;){
                        spacesAway++;
                        destination = mailbox[mailbox64[destination] + offsets[3][i]];
                        //printf("%d\n", square);
                        //printf("%d\n", destination);
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0){
                            if(cp->piece[destination] == 1 || cp->piece[destination] == 6){
                                if(spacesAway > 1){
                                    break;
                                }
                            }
                            if(cp->piece[destination] == 3){
                                if(cp->color[destination] == -1){
                                    allAttackers[i].push_back(3);
                                }
                                else{
                                    allAttackers[9+i].push_back(3);
                                }
                            }
                            else if(cp->piece[destination] == 5){
                                if(cp->color[destination] == -1){
                                    allAttackers[i].push_back(9);
                                }
                                else{
                                    allAttackers[9+i].push_back(9);
                                }
                            }
                            else if(cp->piece[destination] == 6){
                                if(cp->color[destination] == -1){
                                    allAttackers[i].push_back(100);
                                }
                                else{
                                    allAttackers[9+i].push_back(100);
                                }
                            }
                            else if(cp->piece[destination] == 1){
                                if(cp->color[destination] == -1 && (i==0 || i==1)){
                                    allAttackers[i].push_back(1);
                                }
                                else if (cp->color[destination] == 1 && (i==2 || i==3)){
                                    allAttackers[9+i].push_back(1);
                                }
                            }
                            else{
                                break;
                            }
                            /*
                            if(cp->color[destination] == cp->toMove){
                                    if(cp->piece[destination] == 3 && smallestAttacker > 300){
                                        //printf("%d\n", destination);
                                        //printf("here\n");
                                        smallestAttacker = 300;
                                        squareOfSmallestAttacker = destination;
                                    }
                                    if(cp->piece[destination] == 5 && smallestAttacker > 900){
                                        smallestAttacker = 900;
                                        squareOfSmallestAttacker = destination;
                                    }
                            }
                            
                            break;
                            */
                        }
                    }
                }
                int spacesAway = 0;
                for(int i=0; i<4; i++){
                    //printf("%d\n",i);
                    for(int destination = square;;){
                        spacesAway++;
                        destination = mailbox[mailbox64[destination] + offsets[4][i]];
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0){
                            if(cp->piece[destination] == 6){
                                if(spacesAway > 1){
                                    break;
                                }
                            }
                            if(cp->piece[destination] == 4){
                                if(cp->color[destination] == -1){
                                    allAttackers[4+i].push_back(5);
                                }
                                else{
                                    allAttackers[13+i].push_back(5);
                                }
                            }
                            else if(cp->piece[destination] == 5){
                                if(cp->color[destination] == -1){
                                    allAttackers[4+i].push_back(9);
                                }
                                else{
                                    allAttackers[13+i].push_back(9);
                                }
                            }
                            else if(cp->piece[destination] == 6){
                                if(cp->color[destination] == -1){
                                    allAttackers[4+i].push_back(100);
                                }
                                else{
                                    allAttackers[13+i].push_back(100);
                                }
                            }
                            else{
                                break;
                            }
                            /*
                            if(cp->color[destination] == cp->toMove){
                                    if(cp->piece[destination] == 4 && smallestAttacker > 500){
                                        //printf("%d\n", destination);
                                        //printf("here\n");
                                        smallestAttacker = 500;
                                        squareOfSmallestAttacker = destination;
                                    }
                                    if(cp->piece[destination] == 5 && smallestAttacker > 900){
                                        smallestAttacker = 900;
                                        squareOfSmallestAttacker = destination;
                                    }
                            }
                            break;
                            */
                        }
                    }
                }
    //printf("here\n");
    /*
    for(int i=0; i<8; i++){
        int destination = mailbox[mailbox64[square] + offsets[5][i]];
        if(destination == -1){
            continue;
        }
        if(cp->color[destination]==cp->toMove && cp->piece[destination] == 6 && smallestAttacker > 10000){
            smallestAttacker = 10000;
            squareOfSmallestAttacker = destination;
        }
        if(cp->toMove==1 && (i==5 || i==7) && cp->color[destination]==cp->toMove && cp->piece[destination] == 1){
            //check for black pawn
            smallestAttacker = 100;
            squareOfSmallestAttacker = destination;
        }
        if(cp->toMove==-1 && (i==0 || i==2) && cp->color[destination]==cp->toMove && cp->piece[destination] == 1){
            //check for white pawn
            smallestAttacker = 100;
            squareOfSmallestAttacker = destination;
        }
    }
    */
    //printf("here\n");
    return allAttackers;
}

bool kingInDanger(chessPosition* cp){
    for(int i=0;i<64; i++){
        if(cp->color[i] == cp->toMove && cp->piece[i] == 6){
            return checkIfSquareInCheck(cp, i);
        }
    }
    //should never happen
    return false;
}

bool checkIfSquareInCheck(chessPosition* cp, int square){
    //printf("here\n");
        for(int i=0; i<8; i++){
            int destination = square;
            destination = mailbox[mailbox64[destination] + offsets[2][i]];
            if(destination == -1){
                continue;
            }
            if(cp->color[destination] == -cp->toMove){
                if(cp->piece[destination] == 2){
                    return true;
                }
            }
        }
        //printf("here\n");
        for(int i=0; i<4; i++){
            //printf("%d\n",i);
                    for(int destination = square;;){
                        destination = mailbox[mailbox64[destination] + offsets[3][i]];
                        //printf("%d\n", square);
                        //printf("%d\n", destination);
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0){
                            if(cp->color[destination] == -cp->toMove){
                                    if(cp->piece[destination] == 3 || cp->piece[destination] == 5){
                                        //printf("%d\n", destination);
                                        //printf("here\n");
                                        return true;
                                    }
                            }
                            break;
                        }
                    }
                }
                for(int i=0; i<4; i++){
                    //printf("%d\n",i);
                    for(int destination = square;;){
                        destination = mailbox[mailbox64[destination] + offsets[4][i]];
                        if(destination == -1){
                            break;
                        }
                        if(cp->color[destination] != 0){
                            if(cp->color[destination] == -cp->toMove){
                                    if(cp->piece[destination] == 4 || cp->piece[destination] == 5){
                                        return true;
                                    }
                            }
                            break;
                        }
                    }
                }
    //printf("here\n");
    for(int i=0; i<8; i++){
        int destination = mailbox[mailbox64[square] + offsets[5][i]];
        if(destination == -1){
            continue;
        }
        if(cp->color[destination]==-cp->toMove && cp->piece[destination] == 6){
            return true;
        }
        if(cp->toMove==1 && (i==0 || i==2) && cp->color[destination]==-cp->toMove && cp->piece[destination] == 1){
            //check for black pawn
            return true;
        }
        if(cp->toMove==-1 && (i==5 || i==7) && cp->color[destination]==-cp->toMove && cp->piece[destination] == 1){
            //check for white pawn
            return true;
        }
    }
    //printf("here\n");
    return false;
}

void afterPosition(chessPosition *cp, resultingPositions rp){
    bool isCastling = false;
    if(rp.fromSquare == 60 && cp->piece[rp.fromSquare] == 6){
        if(rp.toSquare == 58){
            isCastling = true;
            cp->color[56] = 0;
            cp->color[58] = 1;
            cp->color[59] = 1;
            cp->color[60] = 0;
            cp->piece[56] = 0;
            cp->piece[58] = 6;
            cp->piece[59] = 4;
            cp->piece[60] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[0] = 0;
        }
        if(rp.toSquare == 62){
            isCastling = true;
            cp->color[60] = 0;
            cp->color[61] = 1;
            cp->color[62] = 1;
            cp->color[63] = 0;
            cp->piece[60] = 0;
            cp->piece[61] = 4;
            cp->piece[62] = 6;
            cp->piece[63] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[1] = 0;
        }
    }
    if(rp.fromSquare == 4 && cp->piece[rp.fromSquare] == 6){
        if(rp.toSquare == 2){
            isCastling = true;
            cp->color[0] = 0;
            cp->color[2] = -1;
            cp->color[3] = -1;
            cp->color[4] = 0;
            cp->piece[0] = 0;
            cp->piece[2] = 6;
            cp->piece[3] = 4;
            cp->piece[4] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[2] = 0;
        }
        if(rp.toSquare == 6){
            isCastling = true;
            cp->color[4] = 0;
            cp->color[5] = -1;
            cp->color[6] = -1;
            cp->color[7] = 0;
            cp->piece[4] = 0;
            cp->piece[5] = 4;
            cp->piece[6] = 6;
            cp->piece[7] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[3] = 0;
        }
    }
    if(!isCastling){
        //printf("here\n");
        //printf("%d %d\n", rp.toSquare, cp->enPassantTargetSquare);
        if(cp->piece[rp.fromSquare] == 1 && rp.toSquare == cp->enPassantTargetSquare){
            if(cp->toMove == 1){
                //printf("here\n");
                cp->piece[rp.toSquare+8] = 0;
                cp->color[rp.toSquare+8] = 0;
            }
            else{
                cp->piece[rp.toSquare-8] = 0;
                cp->color[rp.toSquare-8] = 0;
            }
        }
        if(cp->piece[rp.fromSquare] == 1 || cp->color[rp.toSquare] == -cp->toMove){
            cp->fiftyMove = 0;
        }
        else{
            cp->fiftyMove = cp->fiftyMove + 1;
        }
        //printf("%d\n",rp.fromSquare/8);
        //printf("%d\n",rp.toSquare/8);
        //printf("%d\n",rp.promotionPiece);
        cp->piece[rp.toSquare] = cp->piece[rp.fromSquare];
        cp->color[rp.toSquare] = cp->color[rp.fromSquare];
        if(cp->piece[rp.fromSquare] == 1 && ((rp.fromSquare/8 == 6 && rp.toSquare/8 == 7) || (rp.fromSquare/8 == 1 && rp.toSquare/8 == 0))){
            cp->piece[rp.toSquare] = rp.promotionPiece;
        }
        if(cp->piece[rp.fromSquare] == 1 && ((rp.fromSquare/8 == 1 && rp.toSquare/8 == 3) || (rp.fromSquare/8 == 6 && rp.toSquare/8 == 4))){
            cp->enPassantTargetSquare = (rp.fromSquare+rp.toSquare)/2;
        }
        else{
            cp->enPassantTargetSquare = -1;
        }
        cp->piece[rp.fromSquare] = 0;
        cp->color[rp.fromSquare] = 0;
        cp->toMove = -cp->toMove;
    }
    if(cp->castling[0] == 1){
            if(cp->color[60] != 1 || cp->piece[60] != 6 || cp->color[56] != 1 || cp->piece[56] != 4){
                cp->castling[0] = 0;
            }
    }
    if(cp->castling[1] == 1){
            if(cp->color[60] != 1 || cp->piece[60] != 6 || cp->color[63] != 1 || cp->piece[63] != 4){
                cp->castling[1] = 0;
            }
    }
    if(cp->castling[2] == 1){
            if(cp->color[4] != -1 || cp->piece[4] != 6 || cp->color[0] != -1 || cp->piece[0] != 4){
                cp->castling[2] = 0;
            }
    }
    if(cp->castling[3] == 1){
            if(cp->color[4] != -1 || cp->piece[4] != 6 || cp->color[7] != -1 || cp->piece[7] != 4){
                cp->castling[3] = 0;
            }
    }
    cp->enPassantTargetSquare = rp.enPassantTargetSquare;
    //printf("here\n");
}

void afterMove(chessPosition *cp, move* rp){
    bool isCastling = false;
    if(rp->fromSquare == 60 && cp->piece[rp->fromSquare] == 6){
        if(rp->toSquare == 58){
            isCastling = true;
            cp->color[56] = 0;
            cp->color[58] = 1;
            cp->color[59] = 1;
            cp->color[60] = 0;
            cp->piece[56] = 0;
            cp->piece[58] = 6;
            cp->piece[59] = 4;
            cp->piece[60] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[0] = 0;
        }
        if(rp->toSquare == 62){
            isCastling = true;
            cp->color[60] = 0;
            cp->color[61] = 1;
            cp->color[62] = 1;
            cp->color[63] = 0;
            cp->piece[60] = 0;
            cp->piece[61] = 4;
            cp->piece[62] = 6;
            cp->piece[63] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[1] = 0;
        }
    }
    if(rp->fromSquare == 4 && cp->piece[rp->fromSquare] == 6){
        if(rp->toSquare == 2){
            isCastling = true;
            cp->color[0] = 0;
            cp->color[2] = -1;
            cp->color[3] = -1;
            cp->color[4] = 0;
            cp->piece[0] = 0;
            cp->piece[2] = 6;
            cp->piece[3] = 4;
            cp->piece[4] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[2] = 0;
        }
        if(rp->toSquare == 6){
            isCastling = true;
            cp->color[4] = 0;
            cp->color[5] = -1;
            cp->color[6] = -1;
            cp->color[7] = 0;
            cp->piece[4] = 0;
            cp->piece[5] = 4;
            cp->piece[6] = 6;
            cp->piece[7] = 0;
            cp->fiftyMove = cp->fiftyMove + 1;
            cp->toMove = -cp->toMove;
            cp->enPassantTargetSquare = -1;
            cp->castling[3] = 0;
        }
    }
    if(!isCastling){
        //printf("here\n");
        //printf("%d %d\n", rp->toSquare, cp->enPassantTargetSquare);
        if(cp->piece[rp->fromSquare] == 1 && rp->toSquare == cp->enPassantTargetSquare){
            if(cp->toMove == 1){
                //printf("here\n");
                cp->piece[rp->toSquare+8] = 0;
                cp->color[rp->toSquare+8] = 0;
            }
            else{
                cp->piece[rp->toSquare-8] = 0;
                cp->color[rp->toSquare-8] = 0;
            }
        }
        if(cp->piece[rp->fromSquare] == 1 || cp->color[rp->toSquare] == -cp->toMove){
            cp->fiftyMove = 0;
        }
        else{
            cp->fiftyMove = cp->fiftyMove + 1;
        }
        cp->piece[rp->toSquare] = cp->piece[rp->fromSquare];
        cp->color[rp->toSquare] = cp->color[rp->fromSquare];
        if(cp->piece[rp->fromSquare] == 1 && ((rp->fromSquare/8 == 6 && rp->toSquare/8 == 7) || (rp->fromSquare/8 == 1 && rp->toSquare/8 == 0))){
            cp->piece[rp->toSquare] = rp->promotionPiece;
        }
        if(cp->piece[rp->fromSquare] == 1 && ((rp->fromSquare/8 == 1 && rp->toSquare/8 == 3) || (rp->fromSquare/8 == 6 && rp->toSquare/8 == 4))){
            cp->enPassantTargetSquare = (rp->fromSquare+rp->toSquare)/2;
        }
        else{
            cp->enPassantTargetSquare = -1;
        }
        cp->piece[rp->fromSquare] = 0;
        cp->color[rp->fromSquare] = 0;
        cp->toMove = -cp->toMove;
    }
    if(cp->castling[0] == 1){
            if(cp->color[60] != 1 || cp->piece[60] != 6 || cp->color[56] != 1 || cp->piece[56] != 4){
                cp->castling[0] = 0;
            }
    }
    if(cp->castling[1] == 1){
            if(cp->color[60] != 1 || cp->piece[60] != 6 || cp->color[63] != 1 || cp->piece[63] != 4){
                cp->castling[1] = 0;
            }
    }
    if(cp->castling[2] == 1){
            if(cp->color[4] != -1 || cp->piece[4] != 6 || cp->color[0] != -1 || cp->piece[0] != 4){
                cp->castling[2] = 0;
            }
    }
    if(cp->castling[3] == 1){
            if(cp->color[4] != -1 || cp->piece[4] != 6 || cp->color[7] != -1 || cp->piece[7] != 4){
                cp->castling[3] = 0;
            }
    }
    //printf("here\n");
}

void reverseMove(chessPosition *cp, move* rp){
    //printf("here\n");
    cp->toMove = -cp->toMove;
    if(rp->isCastling){
            if(rp->toSquare == 58){
                cp->color[56] = 1;
                cp->color[58] = 0;
                cp->color[59] = 0;
                cp->color[60] = 1;
                cp->piece[56] = 4;
                cp->piece[58] = 0;
                cp->piece[59] = 0;
                cp->piece[60] = 6;
                cp->castling[0] = 1;
            }
            if(rp->toSquare == 62){
                cp->color[60] = 1;
                cp->color[61] = 0;
                cp->color[62] = 0;
                cp->color[63] = 1;
                cp->piece[60] = 6;
                cp->piece[61] = 0;
                cp->piece[62] = 0;
                cp->piece[63] = 4;
                cp->castling[1] = 1;
            }
            if(rp->toSquare == 2){
                cp->color[0] = -1;
                cp->color[2] = 0;
                cp->color[3] = 0;
                cp->color[4] = -1;
                cp->piece[0] = 4;
                cp->piece[2] = 0;
                cp->piece[3] = 0;
                cp->piece[4] = 6;
                cp->castling[2] = 1;
            }
            if(rp->toSquare == 6){
                cp->color[4] = -1;
                cp->color[5] = 0;
                cp->color[6] = 0;
                cp->color[7] = -1;
                cp->piece[4] = 6;
                cp->piece[5] = 0;
                cp->piece[6] = 0;
                cp->piece[7] = 4;
                cp->castling[3] = 1;
            }
    }
    else{
        //printf("here\n");
        //printf("%d %d\n", rp->toSquare, cp->enPassantTargetSquare);
        cp->piece[rp->fromSquare] = cp->piece[rp->toSquare];
        cp->color[rp->fromSquare] = cp->color[rp->toSquare];
        if(rp->isPromotion){
            cp->piece[rp->fromSquare] = 1;
        }
        if(rp->isEnPassant){
            if(cp->toMove == 1){
                //printf("here\n");
                cp->piece[rp->toSquare+8] = 1;
                cp->color[rp->toSquare+8] = -1;
                cp->piece[rp->toSquare] = 0;
                cp->color[rp->toSquare] = 0;
            }
            else{
                cp->piece[rp->toSquare-8] = 1;
                cp->color[rp->toSquare-8] = 1;
                cp->piece[rp->toSquare] = 0;
                cp->color[rp->toSquare] = 0;
            }
        }
        else{
            int capturedPiece = rp->capturedPiece;
            if(capturedPiece != 0){
                cp->color[rp->toSquare] = -cp->toMove;
                cp->piece[rp->toSquare] = rp->capturedPiece;
            }
            else{
                cp->color[rp->toSquare] = 0;
                cp->piece[rp->toSquare] = 0;
            }
        }
    }
    //printf("here\n");

}

void printBoard(chessPosition* cp){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(cp->color[8*i+j] == -1){
                printf("%d",cp->color[8*i+j]*cp->piece[8*i+j]);
            }
            else{
                printf(" %d",cp->color[8*i+j]*cp->piece[8*i+j]);
            }
            if(j!=8){
                printf(" ");
            }
        }
        printf("\n");
    }
}

void printHashTest(){
    chessPosition* cp = new chessPosition();
    int colorStart[64] = {
            -1,0,-1,0,-1,-1,0,-1,
            -1,-1,-1,0,-1,-1,-1,-1,
            0,0,-1,0,-1,-1,0,0,
            0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,0,0,
            1,1,1,0,1,1,1,1,
            1,0,1,1,1,0,1,1
        };
        int pieceStart[64] = {
            4,0,3,0,6,3,0,4,
            1,1,1,0,1,1,1,1,
            0,0,2,0,5,2,0,0,
            0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,
            0,0,2,0,0,0,0,0,
            1,1,1,0,3,1,1,1,
            4,0,3,5,6,0,2,4
        };
        std::copy(colorStart, colorStart+64, cp->color);
        std::copy(pieceStart, pieceStart+64, cp->piece);
        printf("%ld\n",hashKey(cp));
    delete(cp);
}

void testPosition(chessPosition* cp){
    
    //f4 e3
       int colorStart[64] = {
            0,0,-1,0,-1,0,0,-1,
            0,-1,0,0,0,0,-1,-1,
            0,-1,0,0,0,0,0,0,
            0,0,-1,0,0,-1,0,0,
            -1,-1,1,0,-1,1,0,0,
            0,0,0,0,0,0,0,0,
            1,1,0,1,0,1,1,1,
            1,0,1,0,1,0,1,0
        };
        
       
       int pieceStart[64] = {
            0,0,4,0,6,0,0,4,
            0,3,0,0,0,0,1,1,
            0,3,0,0,6,0,0,0,
            0,0,2,0,0,1,0,0,
            1,1,1,0,1,3,1,0,
            0,0,0,0,0,0,0,0,
            1,1,0,2,0,1,1,1,
            2,0,4,0,4,0,6,0
        };

        cp->castling[3] = 0;

        std::copy(colorStart, colorStart+64, cp->color);
        std::copy(pieceStart, pieceStart+64, cp->piece);
        
}

int main(){
    resetHashTable();
    generateRandom();
    std::string input;
    int fromSquare = 0;
    int toSquare = 0;
    int playAs = 0;
    chessPosition* cp = new chessPosition;

    //testPosition(cp);

    //printHashTest();
    //checkIfSquareInCheck(cp,60);

    printf("Play As? 0=white, 1=black\n");
    std::getline(std::cin, input);
    playAs = std::stoi(input, nullptr,10);

    //playAs = 1;
    //printf("%d\n",playAs);
    if(playAs == 0){
        std::getline(std::cin, input);
        //printf("here\n");
        fromSquare = input[0] - 97 + 64 - 8*(input[1] - 48);
        toSquare = input[3] - 97 + 64 - 8*(input[4] - 48);
        //printf("%c:%c:%c:%c:%c\n",input[0], input[1], input[2], input[3], input[4]);
        //printf("%d %d\n",fromSquare, toSquare);

        //need to get promotion piece for pawn promotions
        resultingPositions rp;
        rp.fromSquare = fromSquare;
        rp.toSquare = toSquare;
        if(input.length() > 5){
            if(input[5] == 'N'){
                rp.promotionPiece = 2;
            }
            if(input[5] == 'B'){
                rp.promotionPiece = 3;
            }
            if(input[5] == 'R'){
                rp.promotionPiece = 4;
            }
            if(input[5] == 'Q'){
                rp.promotionPiece = 5;
            }
        }
        afterPosition(cp,rp);
    }

    while(true){
    clock_t timer;
    timer = clock();
            
    resultingPositions dummy;
    resultingPositions rp = generateMoves(cp, 100000*cp->toMove, false, dummy);

    if(printTotalTimeStats){
        printf("total %.2fs\n", (float)(clock()-timer)/CLOCKS_PER_SEC);
    }

    resetHashTable();
    printf("%c%d %c%d",rp.fromSquare % 8 + 97,8-rp.fromSquare / 8,rp.toSquare % 8 + 97,8-rp.toSquare / 8);
    if(rp.promotionPiece != 0){
        char pieces[6] = {' ', ' ', 'N', 'B', 'R', 'Q'};
        printf("%c",pieces[rp.promotionPiece]);
    }
    printf("\n");
    printf("%.2f\n",((double)rp.eval)/100);

    //printBoard(cp);
    afterPosition(cp,rp);

    //printBoard(cp);

    cp->depth = 0;
    std::getline(std::cin, input);
    fromSquare = input[0] - 97 + 64 - 8*(input[1] - 48);
    toSquare = input[3] - 97 + 64 - 8*(input[4] - 48);
    //printf("%c:%c:%c:%c:%c\n",input[0], input[1], input[2], input[3], input[4]);
    //printf("%d %d\n",fromSquare, toSquare);
    if(fromSquare == 64 || toSquare == 64){
        //input i1 to break
        break;
    }

    //need to get promotion piece for pawn promotions
    resultingPositions rp2;
    rp2.fromSquare = fromSquare;
    rp2.toSquare = toSquare;
    if(input.length() > 5){
            if(input[5] == 'N'){
                rp2.promotionPiece = 2;
            }
            if(input[5] == 'B'){
                rp2.promotionPiece = 3;
            }
            if(input[5] == 'R'){
                rp2.promotionPiece = 4;
            }
            if(input[5] == 'Q'){
                rp2.promotionPiece = 5;
            }
        }
    afterPosition(cp,rp2);
    }
}
