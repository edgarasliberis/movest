/*
 * Copyright (c) 2016 Edgar Liberis
 *
 * Movest decoder based on extract_mvs.c FFmpeg example.
 */

/*
 * Copyright (c) 2012 Stefano Sabatini
 * Copyright (c) 2014 Clément Bœsch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <libavutil/motion_vector.h>
#include <libavformat/avformat.h>
#include <getopt.h>
#include <stdbool.h>
#include "movestlib/movest_connector.h"

static AVFormatContext *fmt_ctx = NULL;
static AVCodecContext *video_dec_ctx = NULL;
static AVStream *video_stream = NULL;

static int video_stream_idx = -1;
static AVFrame *frame = NULL;
static AVPacket pkt;

static int decode_packet(int *got_frame)
{
    int decoded = pkt.size;

    *got_frame = 0;

    if (pkt.stream_index == video_stream_idx) {
        int ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error decoding video frame (%s)\n", av_err2str(ret));
            return ret;
        }

        if (*got_frame) {
            av_frame_get_side_data(frame, AV_FRAME_DATA_MOTION_VECTORS);
        }
    }

    return decoded;
}

static int open_codec_context(const char* video_file, int *stream_idx,
                              AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret;
    AVStream *st;
    AVCodecContext *dec_ctx = NULL;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), video_file);
        return ret;
    } else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];

        /* find decoder for the stream */
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Init the video decoder */
        av_dict_set(&opts, "flags2", "+export_mvs", 0);
        if ((ret = avcodec_open2(dec_ctx, dec, &opts)) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;
}

static int decode_video(const char* video_file) {
    int ret = 0, got_frame;
    av_register_all();

    if (avformat_open_input(&fmt_ctx, video_file, NULL, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not open source file %s\n", video_file);
        return 1;
    }

    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not find stream information\n");
        return 1;
    }

    if (open_codec_context(video_file, &video_stream_idx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];
        video_dec_ctx = video_stream->codec;
    }

    if (!video_stream) {
        av_log(NULL, AV_LOG_ERROR, "Could not find video stream in the input, aborting\n");
        ret = 1;
        goto end;
    }

    frame = av_frame_alloc();
    if (!frame) {
        av_log(NULL, AV_LOG_ERROR, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    /* read frames from the file */
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        AVPacket orig_pkt = pkt;
        do {
            ret = decode_packet(&got_frame);
            if (ret < 0)
                break;
            pkt.data += ret;
            pkt.size -= ret;
        } while (pkt.size > 0);
        av_packet_unref(&orig_pkt);
    }

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame);
    } while (got_frame);

    end:
    avcodec_close(video_dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_frame_free(&frame);

    return ret < 0? ret : 0;
}

bool is_supported_algorithm(const char *algname) {
    return strcmp(algname, "hidenseek") == 0
           || strcmp(algname, "rand-hidenseek") == 0
           || strcmp(algname, "dumpmvs") == 0
           || strcmp(algname, "msteg") == 0
           || strcmp(algname, "f3") == 0
           || strcmp(algname, "f4") == 0
           || strcmp(algname, "mvsteg") == 0
           || strcmp(algname, "outguess1") == 0
           || strcmp(algname, "xualg") == 0;
}

int main(int argc, char **argv)
{
    int ret = 0, val;
    static int encrypt_flag = 0;
    char *algorithm = NULL;
    char *password = NULL;
    uint32_t capacity = 0;
    uint32_t file_size = 0;

    // Parse user options
    static struct option long_options[] =
    {
        {"decrypt", no_argument, &encrypt_flag, 1},
        {"algorithm", required_argument, 0,     'a'},
        {"password", required_argument, 0,      'p'},
        {"capacity", required_argument, 0,      'c'},
        {"file-size", required_argument, 0,     'f'},
        {"help", no_argument, 0,                'h'}
    };

    int option_index = -1, c;
    while((c = getopt_long(argc, argv, "a:p:c:f:h", long_options, &option_index)) != -1) {
        switch(c) {
            case 0:
                // Flag handled
                break;
            case 'a':
                if(!optarg || is_supported_algorithm(optarg) == 0) {
                    av_log(NULL, AV_LOG_ERROR, "No or unsupported algorithm provided (%s).\n", optarg);
                    return 1;
                }
                algorithm = optarg;
                break;
            case 'p':
                if(!optarg) {
                    av_log(NULL, AV_LOG_ERROR, "-p/--password requires seed data (a string) as an argument.\n");
                    return 1;
                }
                password = optarg;
                break;
            case 'c':
                if(!optarg || atoi(optarg) <= 0) {
                    av_log(NULL, AV_LOG_ERROR, "-c/--capacity requires a positive integer as an argument.\n");
                    return 1;
                }
                capacity = (uint32_t) atoi(optarg);
                break;
            case 'f':
                if(!optarg || atoi(optarg) <= 0) {
                    av_log(NULL, AV_LOG_ERROR, "-f/--file-size output file size in bytes as an argument.\n");
                    return 1;
                }
                file_size = (uint32_t) atoi(optarg);
                break;
            case 'h':
                av_log(NULL, AV_LOG_INFO, "MOVEST Decoder, (c) 2016\n"
                        "Usage: movest_dec -a <algorithm> [--decrypt, -p <password>, -f <file_size>, -c <capacity>]"
                                          "<input_video> <output_file>\n"
                        "\nCommand line arguments:\n"
                        " --decrypt        Perform decryption of the data prior to embedding\n"
                        " -a/--algorithm   An embedding algorithm to use\n"
                        " -p/--password    An encryption password to use\n"
                        " -c/--capacity    The embedding capacity of a video that was reported during the embedding\n"
                        "                  ('rand-hidenseek'/'outguess1' only)\n"
                        " -f/--file-size   The size of the payload\n"
                        "                  ('rand-hidenseek'/'outguess1' only)\n"
                        " -h/--help        Print this help message\n"
                        "\nAvailable algorithm options:\n"
                        " 'dumpmvs' (writes motion vectors to the output file)\n"
                        " 'hidenseek' 'msteg' 'f3' 'f4'\n"
                        " 'mvsteg' 'xualg'\n"
                        " 'rand-hidenseek' 'outguess1'\n");
                return 0;
            default:
                av_log(NULL, AV_LOG_ERROR, "Unknown option provided: %c\n", c);
                return 1;
        }
    }

    const char *video_file = argv[optind++];
    const char *data_file = argv[optind++];
    if(optind != argc) {
        av_log(NULL, AV_LOG_ERROR, "Incorrect number of arguments provided.\n"
                "Usage: movest_dec -a <algorithm> [--decrypt, -p <password>, -f <file_size>, -c <capacity>]"
                "<input_video> <output_file>\n"
                "See --help for more info.");
        return 1;
    }

    if(!algorithm) {
        av_log(NULL, AV_LOG_INFO, "Algorithm not specified. Using 'mvsteg' as a default.\n");
        algorithm = "mvsteg";
    }

    if (encrypt_flag && !password){
        av_log(NULL, AV_LOG_ERROR, "You must provide a password if you want to use crypto. Use -p/--password. \n");
        return 1;
    }

    av_log(NULL, AV_LOG_INFO, "Video file: %s\n", video_file);
    av_log(NULL, AV_LOG_INFO, "Output file: %s\n", data_file);
    av_log(NULL, AV_LOG_INFO, "Algorithm: %s\n", algorithm);
    av_log(NULL, AV_LOG_INFO, "Crypto: %s\n", encrypt_flag ? "ON" : "OFF");
    if(strcmp(algorithm, "rand-hidenseek") == 0 || strcmp(algorithm, "outguess1") == 0) {
        if(!password) {
            av_log(NULL, AV_LOG_INFO, "Password is required by the algorithm. Using default.\n");
            password = "MovestDefaultPassword";
        }
        if(capacity == 0) {
            av_log(NULL, AV_LOG_ERROR, "You must provide capacity parameter that encoder used. Use -c/--capacity. \n");
            return 1;
        }
        if(file_size == 0) {
            av_log(NULL, AV_LOG_ERROR, "You must provide the resulting file size. Use -f/--file-size. \n");
            return 1;
        }

        av_log(NULL, AV_LOG_INFO, "Password: *SET*\n");
        av_log(NULL, AV_LOG_INFO, "Capacity: %d\n", capacity);
    }
    if(file_size != 0) {
        av_log(NULL, AV_LOG_INFO, "Output file size: %d\n", file_size);
    }

    struct alg_options {
        uint32_t byte_capacity;
        uint32_t file_size;
    };
    struct alg_options alg_params = {capacity, file_size};
    movest_init_algorithm(algorithm, &alg_params);

    movest_params p = {
            data_file, encrypt_flag ? MOVEST_ENABLE_ENCRYPTION : MOVEST_NO_PARAMS, password
    };
    movest_init_decoder(&p);

    // Decode the video
    ret = decode_video(video_file);
    if(ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "Video decoding error %d.\n", ret);
        return ret;
    }

    // Parse result
    movest_result res = movest_finalise();
    fprintf(stderr, "Bytes processed: %d\n", res.bytes_processed);

    return res.error_code;
}
