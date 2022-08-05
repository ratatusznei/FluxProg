#include "Block.hpp"

Block :: Block() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    selected = false;
    dragging = false;
    out1_selected = false;
    in1_selected = false;
    out2_selected = false;
    in2_selected = false;
    executing = false;
    toDelete = false;
}

Block :: ~Block() {

}

void Block :: setX(int nx) {
    x = nx;
}

int Block :: getX() {
    return x;
}

void Block :: setY(int ny) {
    y = ny;
}

int Block :: getY() {
    return y;
}

void Block :: setDragging(bool s) {
    dragging = s;
}

bool Block :: getDragging() {
    return dragging;
}

void Block :: setSelected(bool s) {
    selected = s;
}
bool Block :: getSelected() {
    return selected;
}

void Block :: setOut1Selected(bool s) {
    out1_selected = s;
}

bool Block :: getOut1Selected() {
    return out1_selected;
}

void Block :: setIn1Selected(bool s) {
    in1_selected = s;
}

bool Block :: getIn1Selected() {
    return in1_selected;
}

void Block :: setOut2Selected(bool s) {
    out2_selected = s;
}

bool Block :: getOut2Selected() {
    return out2_selected;
}

void Block :: setIn2Selected(bool s) {
    in2_selected = s;
}

bool Block :: getIn2Selected() {
    return in2_selected;
}

int Block :: getType() {
    return type;
}

void Block :: setTypeOfSensor(int t) {

}
int Block :: getTypeOfSensor() {
    return 0;
}

void Block :: setParameter1(int p) {

}
int Block :: getParameter1() {
    return 0;
}
void Block :: setParameter2(int p) {

}
int Block :: getParameter2() {
    return 0;
}

void Block :: setFunction(int f) {

}
int Block :: getFunction() {
    return 0;
}

void Block :: setWidth(int w) {
    width = w;
}

int Block :: getWidth() {
    return width;
}
void Block :: setHeight(int h) {
    height = h;
}
int Block :: getHeight() {
    return height;
}
void Block :: setNext1(Block *b) {

}
Block* Block :: getNext1() {

    return NULL;
}
void Block :: setNext2(Block *b) {

}
Block* Block :: getNext2() {
    return NULL;
}
void Block :: setPrevious1(Block *b) {

}
Block* Block :: getPrevious1() {
    return NULL;
}
void Block :: setPrevious2(Block *b) {

}
Block* Block :: getPrevious2() {
    return NULL;
}
int Block :: getPointIn1X() {
    return 0;
}
int Block :: getPointIn1Y() {
    return 0;
}
int Block :: getPointIn2X() {
    return 0;
}
int Block :: getPointIn2Y() {
    return 0;
}
int Block :: getPointOut1X() {
    return 0;
}
int Block :: getPointOut1Y() {
    return 0;
}
int Block :: getPointOut2X() {
    return 0;
}
int Block :: getPointOut2Y() {
    return 0;
}
void Block :: setName(const char *n) {
    name = n;
}
char* Block :: getName() {
    return (char*) name;
}
Block* Block :: getExecutingNext() {
    return NULL;
}
void Block :: reset_loop_variables() {

}
int Block :: getCommand() {
    return 0;
}
void Block :: setExecuting(bool s) {
    executing = s;
}
bool Block :: getExecuting() {
    return executing;
}
void Block :: setDelete(bool t) {
    toDelete = t;
}
bool Block :: getDelete() {
    return toDelete;
}
void Block :: setValue(int v) {

}
int Block :: getUnit() {
    return 0;
}
int Block :: getTen() {
    return 0;
}
void Block :: setID(int i) {
    id = i;
}
int Block :: getID() {
    return id;
}
int Block :: setFirstSlotAttr(int n, int* var_int = NULL, bool* var_bool = NULL) {
    return 0;
}
int Block :: setSecondSlotAttr(int n, int* var_int = NULL, bool* var_bool = NULL, int value = 0) {
    return 0;
}
int Block :: setFirstSlotMath(int n = 0, int* var_int = NULL, bool* var_bool = NULL){
    return 0;
}
int Block :: setSecondSlotMath(int n = 0, int* var_int = NULL, bool* var_bool = NULL, int value = 0){
    return 0;
}
int Block :: setThirdSlotMath(int n = 0, int value = 0){
    return 0;
}
int Block :: setFourthSlotMath(int n = 0, int* var_int = NULL, bool* var_bool = NULL, int value = 0){
    return 0;
}
int Block :: setFirstSlotConditional(int n = 0, int* var_int = NULL, bool* var_bool = NULL){
    return 0;
}
int Block :: setSecondSlotConditional(int value = 0){
    return 0;
}
int Block :: setThirdSlotConditional(int n = 0, int* var_int = NULL, bool* var_bool = NULL, int value = 0){
    return 0;
}
int Block :: setFirstSlotWhile(int n = 0, int* var_int = NULL, bool* var_bool = NULL){
    return 0;
}
int Block :: setSecondSlotWhile(int value = 0){
    return 0;
}
int Block :: setThirdSlotWhile(int n = 0, int* var_int = NULL, bool* var_bool = NULL, int value = 0){
    return 0;
}
Block* Block :: executeFunction(){
    return NULL;
}
int* Block :: returnIntVar_1(){
    return 0;
}
bool* Block :: returnBoolVar_1(){
    return 0;
}
int* Block :: returnIntVar_2(){
    return 0;
}
bool* Block :: returnBoolVar_2(){
    return 0;
}
int Block :: returnStaticValue_2(){
    return 0;
}
int* Block :: returnIntVar_3(){
    return 0;
}
bool* Block :: returnBoolVar_3(){
    return 0;
}
int Block :: returnStaticValue_3(){
    return 0;
}
int* Block :: returnIntVar_4(){
    return 0;
}
bool* Block :: returnBoolVar_4(){
    return 0;
}
int Block :: returnStaticValue_4(){
    return 0;
}
int Block :: returnLogicSymbol_2(){
    return 0;
}
int Block :: returnLogicSymbol_3(){
    return 0;
}
int Block :: returnMathSymbol_3(){
    return 0;
}