#ifndef __HCOMMAND_H
#define __HCOMMAND_H

#include "hexceptions.h"

/** Command classes */
enum HCOMMAND_CLASS
{
	/** No class.

	    Description:
		Can be send anywhere without activating no receivers. No receivers should attempt to handle commands of this class
	*/
	NONE
};

/** Command opcodes */
enum HCOMMAND_OPCODE
{
    /** No Operation

        Description:
        Can be send anywhere to do exatcly nothing

        Options:
        None

        Data.Value (INT32):
        Value is indeterminate and must be ignored

        Length:
        struct + sizeof(HCommandData)
    */
    NOP
};

/** Data union of a command */
union HCommandData {

    /** Integer value carried by the command */
    int32_t Value;

    /** Boolean value carried by the command */
    bool State;

    /** Data carried by the command */
    void *Content;
};

/** Command structure. Contains the opcode and data required by a command */
struct HCommand
{
    /** Command class - define the possible target types */
    int16_t Class;

	/** The command to execute */
	int16_t Opcode;

	/** Length of data, for boolean and integer values, this should be 1
	 * otherwise set to the length of the content not including an eventual terminating
	 * zero byte. (that is. Use the exact length of the content block, and let the receiver
	 * convert it into a zero-terminated buffer if they prefer so.). The length of the content
	 * is in 8bit bytes, no matter what type of data is really encoded in the buffer
	 * (in the future, some commands may carry a serialized object, structured data or other
	 * types of advanced data in the content field. Only the sender and receiver knowns how
	 * to handle the content. All others should handle the content as an oblige blob of bytes.
	 * A special case is the use of the class NONE and opcode NOP, here Length should be set to 0 (zero)
	 */
	int16_t Length;

	/** Data block. Depending on the class and opcode of the command, this block contains either
	 * a 32bit (signed) value, a boolean true/false or a buffer containing a number
	 */
	 HCommandData Data;
};

/* The absolut minimum size of a command, a command that holds either a boolean value or an integer value
 * This is the absolut minimum size of a chunk of data that can be converted to a HCommand struct */
static int HCommandMinimumsSize = sizeof(HCommand);

/** Get the size of a command, including data content */
inline size_t HCommandSize(HCommand* command) {
    return HCommandSize(command);
}

/** Get the size of a command, including data content */
inline size_t HCommandSize(HCommand& command) {
    switch(command.Opcode) {
        case NONE: return HCommandMinimumsSize + 0;

            // Unknownn opcode.
        default: throw new HUnknownCommandOpcodeException("Unknown opcode in call to HCommandSize (or macro HCOMMAND_LENGTH)");
    }
}

#endif
