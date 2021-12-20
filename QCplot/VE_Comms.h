#pragma once

// #include <atlstr.h>
// Namespace of VE_Comms Module for communication between external controllers and Virtual Enviroment
#define DllExport   __declspec( dllexport )
//Virtual Environment Communication class file to communicate between external controller and Virtual Enviroment
class DllExport VE_Comms
{
public:
	/**
	Container of a single Joint rotation command
	@param int FingerNumber -  Individual FingerNumber 0-4 start from Thumb, 6 for Wrist articulation and rotation
	@param int JointNumber - Individual Joint values between 0-2 inclusive from palm to fingertip, 0,1 for wrist.
	*/
	typedef struct
	{
		int FingerNumber;
		int JointNumber;

	} Joint;
	/**
	Container of a single Joint rotation command
	@param Joint JointName  - Selection joint to perform action upon
	@param float X_Rotation - X Rotation to perform
	@param float Y_Rotation - Y Rotation to perform 
	@param float Z_Rotation - Z Rotation to perform
	*/
	typedef struct 
	{
		Joint _Joint;
		float X_Rotation; 
		float Y_Rotation;
		float Z_Rotation; 
	} JointRotation;
	//thumb,index,middle ring,small, 
	//static Joint w0; // Wrist/forearm rotation
	//static Joint w1; // Wrist Articulations 
	/*

	Joint w2 = { 6,2 }; // Wrist Articulations 
	Joint t0 = { 0,0 }; // Thumb Metacarpophalangeal (MP) Joint
	Joint t1 = { 0,1 }; // Thumb Proximal Interphalangeal (PIP) Joint
	Joint t2 = { 0,2 }; // Thumb Distal Interphalangeal (DIP) Joint
	Joint i0 = { 1,0 }; // Thumb Metacarpophalangeal (MP) Joint
	Joint i1 = { 1,1 }; // Index Finger Proximal Interphalangeal (PIP) Joint
	Joint i2 = { 1,2 }; // Index Finger Distal Interphalangeal (DIP) Joint
	Joint m0 = { 2,0 }; // Middle Finger Metacarpophalangeal (MP) Joint
	Joint m1 = { 2,1 }; // Middle Finger Proximal Interphalangeal (PIP) Joint
	Joint m2 = { 2,2 }; // Middle Finger Distal Interphalangeal (DIP) Joint
	Joint r0 = { 3,0 }; // Ring Finger Metacarpophalangeal (MP) Joint
	Joint r1 = { 3,1 }; // Ring Finger Proximal Interphalangeal (PIP) Joint
	Joint r2 = { 3,2 }; // Ring Finger Distal Interphalangeal (DIP) Joint
	Joint s0 = { 4,0 }; // Small Finger Metacarpophalangeal (MP) Joint
	Joint s1 = { 4,1 }; // Small Finger Proximal Interphalangeal (PIP) Joint
	Joint s2 = { 4,2 }; // Small Finger Distal Interphalangeal (DIP) Joint

	*/


	/**
	Formats and sends a joint command.

	@param gesture - the individual digit of the motion class to perform
	@param bool getJointPositions - defaults as false, set to true to return joint angles
	@return either 1 or all Joint Angles completion, dependant upon the boolean value of getJointPositions
	*/
	static char*  SendJoint(JointRotation* JointCommand, bool getJointPositions = false);
	/**
	Formats and sends a joint command with speed modifier.

	@param gesture - the individual digit of the motion class to perform
	@param speed - The speed to perform the motion (ideally <20)
	@param bool getJointPositions - defaults as false, set to true to return joint angles
	@return either 1 or all Joint Angles completion, dependant upon the boolean value of getJointPositions
	*/
	static char*  SendJoint(JointRotation* JointCommand,int speed, bool getJointPositions = false);
	/**
	Formats and sends a series of joint commands.

	@param gesture - the individual digit of the motion class to perform
	@param bool getJointPositions - defaults as false, set to true to return joint angles
	@return either 1 or all Joint Angles completion, dependant upon the boolean value of getJointPositions
	*/
	static char*  SendJoints(JointRotation JointCommand[], bool getJointPositions = false);
	/**
	Formats and sends a series of joint commands with speed modifier.

	@param gesture - the individual digit of the motion class to perform
	@param speed - The speed to perform the motion (ideally <20)
	@param bool getJointPositions - defaults as false, set to true to return joint angles
	@return either 1 or all Joint Angles completion, dependant upon the boolean value of getJointPositions
	*/
	static char*  SendJoints(JointRotation JointCommand[], int speed, bool getJointPositions = false);
	/**
	Formats and sends a single gesture command. 

	@param int gesture - the individual digit of the motion class to perform
	@param bool getJointPositions - defaults as false, set to true to return joint angles
	@returns a char* of either "1" or all Joint Angles, dependant upon the boolean value of getJointPositions
	*/
	static char* SendGesture(int Gesture, bool getJointPositions = false);

	/* Sends a UDP stream to the Virtual enviroment alongside request for retrieving joint positions
	@param char* input - char array to send to enviroment
	@param bool getJointPositions - defaults to false, set to true to return joint positions

	@returns: "1" if getJointPositions is not set, or set to false, otherwise returns joint positions of hand when command is sent. 
	*/
	static char* SendUDP(const char* input,bool getJointPositions = false);


	/* Sends a signal to the Virtual Environment to start training.
	@Returns 1 if successful 
	*/
	static int startTraining();


	// Returns all joint angles at the present time
	static char* getJointAngles();


	static JointRotation GetJointPosition(Joint _joint);

	static JointRotation* GetJointPositions(Joint _joints[]);

	// Initializes the server for sending information to the environment
	static int InitializeServer();

	static int CloseClient();

	static int CloseServer();
};

