#pragma once
extern int scoringStage;
extern int load;
extern int p1;
extern int p2;
extern int p3;
void handleArcade();
void handleTank();
void handleDriveMode(bool driveMode);
void handleDiddy();
void handleCascadeStage();
void handleCascadeScoring();    
void score(int intendedStage);