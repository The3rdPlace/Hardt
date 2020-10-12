#ifndef __HCOMMAND_H
#define __HCOMMAND_H

#include "hexceptions.h"

/** Command classes */
enum H_COMMAND_CLASS
{
	/**
	 * No class.
	 *
	 * Can be send anywhere.
	 *
	 * All receivers must either pass the command on or return true
	 */
	NONE = 0,

	/**
	 * Any class.
	 *
	 * Command opcode operates on any class that supports that opcode
	 */
	ANY = 1,

	/**
	 * Tuner class.
	 *
	 * Command opcode operates on any device containing a tuner, virtual as well as real
	 */
    TUNER = 2,
};

/** Command opcodes */
enum H_COMMAND_OPCODE
{
    /**
     * No Operation
     *
     * Can be send anywhere, with any class.
     *
     * All receivers must either pass the command on or return true
     */
    NOP = 0,

    /**
     * Class = TUNER
     *
     * Opcode sets the center or LO frequency of the tuner (hardware or virtual).
     * Device type defines which oscillator is set.
     *
     * Command data value must be set to the frequency.
     * Since value is an int32, the highest center or LO frequency that can be set
     * is 2.147.483.647 ~= 2.147 GHz
     */
    SET_FREQUENCY = 1,

    /**
     * Class = TUNER, ANY
     *
     * Set gain. Range and valid values depends on the receiver type
     *
     * Command data value must be set to the gain value
     */
    SET_GAIN = 2
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

	/** Length of data, for boolean and integer values, this should be 0
	 * otherwise set to the length of the content not including an eventual terminating
	 * zero byte. (that is. Use the exact length of the content block, and let the receiver
	 * convert it into a zero-terminated buffer if they prefer so.). The length of the content
	 * is in 8bit bytes, no matter what type of data is really encoded in the buffer
	 * (in the future, some commands may carry a serialized object, structured data or other
	 * types of advanced data in the content field. Only the sender and receiver knowns how
	 * to handle the content. All others should handle the content as an oblige blob of bytes.
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
        case NOP:
        case SET_FREQUENCY:
        case SET_GAIN:
            return HCommandMinimumsSize + 0;

        // Unknownn opcode.
        default: throw new HUnknownCommandOpcodeException("Unknown opcode in call to HCommandSize (or macro HCOMMAND_LENGTH)");
    }
}

#endif
