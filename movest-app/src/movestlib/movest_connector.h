//
// Created by el398 on 06/12/15.
//

#ifndef MOVEST_CONNECTOR_H
#define MOVEST_CONNECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Extra options available for @ref movest_params
 */
typedef enum {
    MOVEST_NO_PARAMS = 0,
    MOVEST_DUMMY_PASS = 1,
    MOVEST_ENABLE_ENCRYPTION = 2
} movest_flags;

/**
 * Parameters for a steganography algorithm.
 */
typedef struct {
    const char *datafile; ///< Path to a file for data to be read from / written to.
    movest_flags flags;   ///< Flags @ref movest_flags
    const char *password; ///< Password for encryption
} movest_params;

/**
 * Result report of the encoding or decoding process.
 *
 * Error codes:
 *   * 0 -- no error
 *   * 1 -- file doesn't fit into the video (encoder only)
 */
typedef struct {
    unsigned int bytes_processed; ///< Number of (full) bytes processed
    int error_code;               ///< Error code (see @ref movest_result)
} movest_result;

/**
 * Set up the library to use a particular algorithm.
 * This is the first step of initialisation and must be followed by
 * @ref movest_init_encoder or @ref movest_init_decoder.
 * Currently available options:
 *     - 'hidenseek'
 *     - 'dumpmvs' (dec) / 'dummypass' (enc)
 *     - 'f3'
 *     - 'f4'
 *     - 'msteg'
 *     - 'mvsteg'
 *     - 'outguess1'
 *     - 'rand-hidenseek'
 *     - 'xualg'
 *
 * @param algname Name of the algorithm from the options above.
 * @param algParams Additional parameters for algorithm of your choice.
 */
void movest_init_algorithm(const char *algname, void* alg_params = 0);

/**
 * Initialise the library for encoding.
 * Must be done after @ref movest_init_algorithm.
 */
void movest_init_encoder(movest_params *params);

/**
 * Initialise the library for decoding.
 * Must be done after @ref movest_init_algorithm.
 */
void movest_init_decoder(movest_params *params);

/**
 * Gather results of the embedding / extracting process (see @ref movest_result).
 */
movest_result movest_finalise();

/**
 * Method called by FFmpeg to feed in motion vector data during encoding.
 * (parameters match those available as variables at the point
 * of injection).
 */
void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);

/**
 * Method called by FFmpeg to feed in motion vector data during decoding.
 * (parameters match those available as variables at the point
 * of injection).
 */
void movest_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height, int mv_stride,
                   int mb_stride);

#ifdef __cplusplus
}
#endif

#endif //MOVEST_CONNECTOR_H
