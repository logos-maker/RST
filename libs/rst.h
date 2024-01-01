// The RST header file

#ifdef __x86_64
	typedef int64_t plugPtr;
#else
	typedef int32_t plugPtr;
#endif

enum { LEFT, RIGHT }; // 0 for left audio channel, 1 for right audio channel.
#define UNKNOWN 0
#define EFFECT_UNIT 1
#define SYNTHESIZER 2 // 0 for unknown, 1 for effect unit, 2 for synthesizer.

//typedef int64_t plugPtr;
typedef struct plugHeader plugHeader;
struct plugHeader{ // The ABI for audio effect plugins
	int32_t magicNumber;
	plugPtr (*plugInstructionDecoderFunc)(plugHeader* effect, int32_t opcode, int32_t index, plugPtr value, void* ptr, float opt);
	void(*deprecatedProcess)(void); // not used, is deprecated
        void  (*plugSetParameterFunc)(plugHeader* effect, int32_t index, float parameter);
        float (*plugGetParameterFunc)(plugHeader* effect, int32_t index);
	int32_t number_of_programs;     // number of programs in plug
	int32_t number_of_params;	// all programs are assumed to have numParams parameters
	int32_t number_of_inputs;	// number of audio inputs in plug
	int32_t number_of_outputs;	// number of audio outputs in plug
	int32_t flags;			// see plugPropertiesFlags
	plugPtr reserved_for_host[2];
	int32_t initialDelay;	        // plug latency in samples
	int32_t deprecated1;
	int32_t deprecated2;
	float   deprecated3;
	void* object;
	void* user;
	int32_t uniqueID; // Depricated as plugs don't follow it.
	int32_t version;  // Version of this plug, to force updates about plug in host.
        void (*plugProcessFloatFunc) (plugHeader* effect, float** inputs, float** outputs, int32_t sampleFrames); // Used by host to make plug process 32bit audio buffers (float).
        void (*plugProcessDoubleFunc)(plugHeader* effect, double** inputs, double** outputs, int32_t sampleFrames);// Used by host to make plug process 64bit audio buffers(double).
	char reserved3_for_future[56];// reserved
};
typedef plugPtr (*hostCallback) (plugHeader* effect, int32_t opcode, int32_t index, plugPtr value, void* ptr, float opt); 

enum plugPropertiesFlags{
        hasEditor	= 1,   	// Plug has an editor window
	hasFloatAudio	= 16,  	// Plug has function for writing audio out over the audio in buffer in 32bit floating point (float).
	hasSaveState	= 32,  	// internal state in plug can be copied to host, for saving and loading audio projects in host.
	hasDoubleAudio	= 4096,	// Plug has function for writing audio out over the audio in buffer in 64bit floating point (double).
	hasSynth	= 256, 	// Plug is a synthesizer.
};
enum opcodes{
        plugEditorOpen=14,        	// Attention: Is called when the DAW whants the plug to open window.
        plugEditorClose=15,       	// Attention: After you answered this call, then the window is closed by DAW.
        plugEditorSize=13,		// Width and hight of the editor window.
        plugEditRedraw=19,		// Host tells plug to redraw graphics.
        plugOpen=0,	           	// Host warns it will start to use this plug instance before it does.
        plugClose=1,    	      	// Host warns that it will end the usage of this plug instance.
        plugGetParamText=7,     	// Host asks for a text to display for the value of a parameter.
        plugGetParamName=8,     	// Host asks for the name of a parameter in plug.
        plugSetSampleRate=10,   	// The host tells the plug what sample rate that is currently used.
        plugGetState=23,		// Host asks for an adress and the length of a data chunk that the host shall save for the plug.
        plugSetState=24,		// Host asks for an adress and length of the data segment that the host shall fill to restore an old state.
        plugCanBeAutomated =26,		// For the host to check if it can automate a certain parameter.
        plugGetVendorString=47,		// For the host to ask for the name of the author of the plug.
        plugGetProductString=48,	// For the host to ask for the name of the plug.	
        plugSupports=51,		// For the host to ask if the plug supports a sertain functionallity, the name of the functionallity is supplied in a text string by the host.
        plugGetVersion=58,		// Asks the plug what version of the ABI that was designed for.
	plugGetProgramNameIndexed=29,	// Gets the preset name by index, Bitwig uses this.
	plugGetProgramName=5,		// Alternative to plugGetProgramNameIndexed.
	plugSetProgram=2,		// Change plug preset program
	plugGetProgram=3,		// Ask plug for program number
	plugGetPlugCategory=35,		// Ask if it's a effect unit or a synthesizer. Other categories exists but is futile.
	plugProcessMIDI=25		// Get MIDI events
};
char* opcode_name[] = {
	[plugEditorOpen] = "plugEditorOpen",		// Attention: Is called when the DAW whants the plug to open window.
        [plugEditorClose] = "plugEditorClose",		// Attention: After you answered this call, then the window is closed by DAW.
        [plugEditorSize] = "plugEditorSize",		// Width and hight of the editor window.
        [plugEditRedraw] = "plugEditRedraw",		// Host tells plug to redraw graphics.
        [plugOpen] = "plugOpen",	           	// Host warns it will start to use this plug instance before it does.
        [plugClose] = "plugClose",    	      		// Host warns that it will end the usage of this plug instance.
        [plugGetParamText] = "plugGetParamText",     	// Host asks for a text to display for the value of a parameter.
        [plugGetParamName] = "plugGetParamName",     	// Host asks for the name of a parameter in plug.
        [plugSetSampleRate] = "plugSetSampleRate",   	// The host tells the plug what sample rate that is currently used.
        [plugGetState] = "plugGetState",		// Host asks for an adress and the length of a data chunk that the host shall save for the plug.
        [plugSetState] = "plugSetState",		// Host asks for an adress and length of the data segment that the host shall fill to restore an old state.
        [plugCanBeAutomated] = "plugCanBeAutomated",	// For the host to check if it can automate a certain parameter.
        [plugGetVendorString] = "plugGetVendorString",	// For the host to ask for the name of the author of the plug.
        [plugGetProductString] = "plugGetProductString",// Gets the preset name by index, Bitwig uses this.	
        [plugSupports] = "plugSupports",		// For the host to ask if the plug supports a sertain functionallity, the name of the functionallity is supplied in a text string by the host.
        [plugGetVersion] = "plugGetVersion",		// Asks the plug what version of the ABI that was designed for.
	[plugGetProgramNameIndexed] = "plugGetProgramNameIndexed",	// Gets the preset name by index, Bitwig uses this.
	[plugGetProgramName] = "plugGetProgramName",	// Alternative to plugGetProgramNameIndexed.
	[plugSetProgram] = "plugSetProgram",		// Change plug preset program
	[plugGetProgram] = "plugGetProgram",		// Ask plug for program number
	[plugGetPlugCategory] = "plugGetPlugCategory",	// Ask if it's a effect unit or a synthesizer. Other categories exists but is futile.
	[plugProcessMIDI] = "plugProcessMIDI",		// Get MIDI events
};
enum opcodesdaw{
	dawAutomate=0,		// send knob value to host 
	dawGetTime=7,		// get time info from host
	dawProcessMIDI=8,	// send MIDI data to host
	dawAutomateStart=43,	// tell host about grabbed knob
	dawAutomateEnd=44,	// tell host about released knob
};
struct ERect{
	int16_t top;    // Set to zero - Specifies the y-coordinate of the upper-left corner of the rectangle.
	int16_t left;   // Set to zero - Specifies the x-coordinate of the upper-left corner of the rectangle.
	int16_t bottom; // Set to window width - Specifies the x-coordinate of the lower-right corner of the rectangle.
	int16_t right;  // Set to window higth - Specifies the y-coordinate of the lower-right corner of the rectangle.
};
typedef struct plugMidiEvent{ // It's a special type of event
	int32_t eventType;		// Set to 1 if MIDI event, 6 is it's a sysex message that is not supported by this struct.
	int32_t eventSize;		// Size of this event in bytes and that is 32.
	int32_t sample_offset;		// offset in number of samples in the audio buffer.
	int32_t realtime_flag;		// Set to 1 if it's not playbacked from a sequencer track. Handle the event immediately if set. if 0 check sample_offset for timing.
	int32_t note_length;		// Note length in samples if availible. Set to 0 if not used.
	int32_t not_used_0;		// Is set to zero.
	char MIDIByte[4];		// 1 to 3 MIDI bytes depending on MIDI message. MIDIByte[3] is not used MIDI v2 possibility?
	char cents_detune;		// -64 to +63 cents 'microtuning'
	char off_velocity;		// Note Off Velocity, usefull when paired with note_length.
	char not_used_1;		// Is set to zero.
	char not_used_2;		// Is set to zero. 
} plugMidiEvent;
struct plugEvents{
	int32_t number_of_events;	// number of plugMidiEvent's in MidiEvent[2]
	plugPtr reserved;		// Is set to zero.
	plugMidiEvent* MIDIMessages[2];	// hold one or two events depending on number_of_events
};
