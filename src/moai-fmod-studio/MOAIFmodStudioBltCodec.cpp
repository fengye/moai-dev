//
//  MOAIFmodStudioBltCodec.c
//  libmoai
//
//  Created by Sylvain Rebaud on 12/20/13.
//
//

/*==============================================================================
 BlueTune Custom Codec for FMOD Studio
 Copyright (c), Plumzi, Inc. 2004-2013.
 
 This example shows how to add a user created Codec callback to decode audio
 data.
 ==============================================================================*/

#include <stdio.h>
#include "fmod.hpp"
#include "BlueTune.h"

/*----------------------------------------------------------------------
|   FMOD_InputStream
+---------------------------------------------------------------------*/
typedef struct {
    FMOD_CODEC_STATE* codec;
    ATX_Position      position;
} FMOD_InputStream;

/*----------------------------------------------------------------------
|   FMOD_InputStream_Read
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_Read(void*     _self,
                      void*     buffer,
                      ATX_Size  bytes_to_read,
                      ATX_Size* bytes_read)
{
    FMOD_InputStream* self = (FMOD_InputStream*)_self;
    
    ATX_Size size;
    FMOD_RESULT result = self->codec->fileread(self->codec->filehandle, buffer, bytes_to_read, &size, 0);
    
    /* update the stream position */
    self->position += size;
    
    if (bytes_read) {
        *bytes_read = size;
    }
	
	if (result == FMOD_ERR_FILE_EOF) {
		return size == 0 ? ATX_ERROR_EOS : ATX_SUCCESS;
	}
    
    return result == FMOD_OK ? ATX_SUCCESS : ATX_ERROR_INTERNAL;
}

/*----------------------------------------------------------------------
|   FMOD_InputStream_Seek
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_Seek(void* _self, ATX_Position position)
{
    FMOD_InputStream* self = (FMOD_InputStream*)_self;
    
    FMOD_RESULT result = self->codec->fileseek(self->codec->filehandle, position, 0);
    if (result == FMOD_OK) {
        self->position = position;
        return ATX_SUCCESS;
    } else {
        return BLT_ERROR_OUT_OF_RANGE;
    }
}

/*----------------------------------------------------------------------
|   FMOD_InputStream_Tell
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_Tell(void* _self, ATX_Position* where)
{
    FMOD_InputStream* self = (FMOD_InputStream*)_self;
    
    *where = self->position;
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   FMOD_InputStream_GetSize
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_GetSize(void* _self, ATX_LargeSize* size)
{
    FMOD_InputStream* self = (FMOD_InputStream*)_self;
    
    *size = self->codec->filesize;
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   FMOD_InputStream_GetAvailable
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_GetAvailable(void* _self, ATX_LargeSize* available)
{
    FMOD_InputStream* self = (FMOD_InputStream*)_self;
    
    *available = self->codec->filesize-self->position;
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   FMOD_InputStream_Interface
+---------------------------------------------------------------------*/
static BLT_InputStream_CallbackInterface
FMOD_InputStreamInterface = {
    FMOD_InputStream_Read,
    FMOD_InputStream_Seek,
    FMOD_InputStream_Tell,
    FMOD_InputStream_GetSize,
    FMOD_InputStream_GetAvailable
};

/*----------------------------------------------------------------------
|   FMOD_InputStream_Create
+---------------------------------------------------------------------*/
static ATX_Result
FMOD_InputStream_Create(FMOD_CODEC_STATE *codec, ATX_InputStream** stream)
{
    ATX_Result result;
    
    /* create a new object */
    FMOD_InputStream* self = (FMOD_InputStream*)ATX_AllocateZeroMemory(sizeof(FMOD_InputStream));
    if (self == NULL) return ATX_ERROR_OUT_OF_MEMORY;
    
    /* keep codec around */
    self->codec = codec;
    
    /* create a wrapper for our instance */
    *stream = BLT_InputStreamWrapper_Create(self, &FMOD_InputStreamInterface);
    
    return ATX_SUCCESS;
}

/*----------------------------------------------------------------------
|   BLT_Codec
+---------------------------------------------------------------------*/
typedef struct
{
    ATX_InputStream* input_object;
    BLT_Decoder*     decoder;
    BLT_MediaPacket* last_packet;
} BLT_Codec;

static FMOD_CODEC_WAVEFORMAT bltwaveformat;

FMOD_RESULT F_CALLBACK bltopen(FMOD_CODEC_STATE *codec, FMOD_MODE usermode, FMOD_CREATESOUNDEXINFO *userexinfo)
{
    BLT_Result result;
	
	/* rewind file, older version of fmod used to do it for us beforehand */
	FMOD_RESULT res = codec->fileseek(codec->filehandle, 0, 0);
    
    /* create an input object to be called back */
    ATX_InputStream* input_object = NULL;
    result = FMOD_InputStream_Create(codec, &input_object);
    if (ATX_FAILED(result)) {
        fprintf(stderr, "ERROR: failed to open input file (%d)\n", result);
        return FMOD_ERR_INTERNAL;
    }
    
    /* create a decoder */
    BLT_Decoder* decoder = NULL;
    BLT_Decoder_Create(&decoder);
    
    BLT_Codec* data = (BLT_Codec*)malloc(sizeof(BLT_Codec));
    data->decoder = decoder;
    data->input_object = input_object;
    data->last_packet = NULL;
    
    /* register builtin modules */
    BLT_Decoder_RegisterBuiltins(decoder);
    
    /* construct the input name */
    char input_name[64];
    sprintf(input_name, "callback-input:%lld", (long long)ATX_POINTER_TO_LONG(input_object));
    
    /* open the input */
    /* IMPORTANT NOTE: this example is hardcoded to an input mime type audio/mp4,
     change to different type if you're not using MP4 */
    result = BLT_Decoder_SetInput(decoder, input_name, "audio/mp4");
    if (BLT_FAILED(result)) {
        fprintf(stderr, "ERROR: SetInput failed (%d)\n", result);
        return FMOD_ERR_INTERNAL;
    }
//    result = BLT_Decoder_SetInput(decoder, input_name, "audio/mp3");
//    if (BLT_FAILED(result)) {
//        fprintf(stderr, "ERROR: SetInput failed (%d)\n", result);
//        return FMOD_ERR_INTERNAL;
//    }
    
    /* set the output to be a memory node */
    result = BLT_Decoder_SetOutput(decoder, "memory", "audio/pcm");
    if (BLT_FAILED(result)) {
        fprintf(stderr, "ERROR: SetOutput failed (%d)\n", result);
        return FMOD_ERR_INTERNAL;
    }
    
    /* get a reference to the output node */
    BLT_MediaNode* output_node = NULL;
    BLT_Decoder_GetOutputNode(data->decoder, &output_node);
    
    /* get the BLT_PacketProducer interface of the output node */
    BLT_PacketProducer* packet_source = ATX_CAST(output_node, BLT_PacketProducer);
    
    /* determine stream sample rate and duration */
    BLT_DecoderStatus status;
    const BLT_PcmMediaType* media_type;
    do {
        result = BLT_Decoder_PumpPacket(data->decoder);
        if (BLT_SUCCEEDED(result)) {
            BLT_PacketProducer_GetPacket(packet_source, &data->last_packet);
            if (data->last_packet) {
                result = BLT_MediaPacket_GetMediaType(data->last_packet, (const BLT_MediaType**)&media_type);
            }
        }
    } while (BLT_SUCCEEDED(result) && !data->last_packet);
    
    if (BLT_FAILED(result) || BLT_FAILED(BLT_Decoder_GetStatus(data->decoder, &status))) {
        return FMOD_ERR_FORMAT;
    }
	
	BLT_Decoder_GetStatus(data->decoder, &status);
    
    bltwaveformat.channels     = media_type->channel_count;
    bltwaveformat.format       = FMOD_SOUND_FORMAT_PCM16;
    bltwaveformat.frequency    = media_type->sample_rate;
    bltwaveformat.blockalign   = bltwaveformat.channels * media_type->bits_per_sample / 8;          /* 2 = 16bit pcm */
    bltwaveformat.lengthbytes  = codec->filesize;
    bltwaveformat.lengthpcm    = bltwaveformat.frequency * status.stream_info.duration / 1000.;   /* bytes converted to PCM samples */;
    
    codec->numsubsounds = 0;
    codec->waveformat   = &bltwaveformat;
    codec->plugindata   = data;                    /* user data value */
	codec->waveformatversion = FMOD_CODEC_WAVEFORMAT_VERSION;
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK bltclose(FMOD_CODEC_STATE *codec)
{
    BLT_Codec* data = (BLT_Codec*)codec->plugindata;
    if (data != NULL) {
        if (data->last_packet) {
            BLT_MediaPacket_Release(data->last_packet);
        }
        
        /* cleanup */
        ATX_RELEASE_OBJECT(data->input_object);
        BLT_Decoder_Destroy(data->decoder);
        
        free(data);
    }
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK bltread(FMOD_CODEC_STATE *codec, void *buffer, unsigned int size, unsigned int *read)
{
    BLT_Result result;
    unsigned int buffer_offset = 0;
    
    BLT_Codec* data = (BLT_Codec*)codec->plugindata;
    if (data->decoder == NULL) {
        fprintf(stderr, "ERROR: Invalid plugindata\n");
        return FMOD_ERR_INVALID_HANDLE;
    }
    
    /* reset input params */
    if (read) {
        *read = 0;
    }
    
    /* use data from last packet if any */
    if (data->last_packet != NULL) {
        unsigned int payload_size = BLT_MediaPacket_GetPayloadSize(data->last_packet);
        unsigned int bytes_to_read = payload_size <= size ? payload_size : size;
        memcpy(buffer, BLT_MediaPacket_GetPayloadBuffer(data->last_packet), bytes_to_read);
        
        /* update offset and release packet if empty */
        BLT_MediaPacket_SetPayloadOffset(data->last_packet, BLT_MediaPacket_GetPayloadOffset(data->last_packet) + bytes_to_read);
        if (BLT_MediaPacket_GetPayloadSize(data->last_packet) == 0) {
            BLT_MediaPacket_Release(data->last_packet);
            data->last_packet = NULL;
        }
        
        size -= bytes_to_read;
        buffer_offset += bytes_to_read;
        
        if (read) {
            *read += bytes_to_read;
        }
    }
    
    /* return if no data left to read */
    if (size == 0) {
        return FMOD_OK;
    }
    
    /* get a reference to the output node */
    BLT_MediaNode* output_node = NULL;
    BLT_Decoder_GetOutputNode(data->decoder, &output_node);
    
    /* get the BLT_PacketProducer interface of the output node */
    BLT_PacketProducer* packet_source = ATX_CAST(output_node, BLT_PacketProducer);
    
    do {
        result = BLT_Decoder_PumpPacket(data->decoder);
        if (BLT_SUCCEEDED(result)) {
            BLT_MediaPacket* packet = NULL;
            BLT_PacketProducer_GetPacket(packet_source, &packet);
            if (packet) {
                const BLT_PcmMediaType* media_type;
                BLT_MediaPacket_GetMediaType(packet, (const BLT_MediaType**)&media_type);
                unsigned int payload_size = BLT_MediaPacket_GetPayloadSize(packet);
                
//                printf("PACKET: sample_rate=%d, channels=%d, bits_per_sample=%d, size=%d\n",
//                       media_type->sample_rate,
//                       media_type->channel_count,
//                       media_type->bits_per_sample,
//                       payload_size);
                
                unsigned int bytes_to_read = payload_size <= size ? payload_size : size;
                memcpy((char *)buffer + buffer_offset, BLT_MediaPacket_GetPayloadBuffer(packet), bytes_to_read);
                
                size -= bytes_to_read;
                buffer_offset += bytes_to_read;
                if (read) {
                    *read += bytes_to_read;
                }
                
                /* advance packet offset with ready data size */
                BLT_MediaPacket_SetPayloadOffset(packet, BLT_MediaPacket_GetPayloadOffset(packet) + bytes_to_read);
                
                /* keep packet around if we didn't read everything from it this time */
                if (BLT_MediaPacket_GetPayloadSize(packet) > 0) {
                    BLT_MediaPacket_AddReference(packet);
                    data->last_packet = packet;
                }
                
                /* don't forget to release the packet */
                BLT_MediaPacket_Release(packet);
            }
        }
    } while (BLT_SUCCEEDED(result) && size > 0);
    
    if (NPT_FAILED(result)) {
        return result == BLT_ERROR_EOS ? FMOD_ERR_FILE_EOF : FMOD_ERR_INTERNAL;
    }
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK bltsetposition(FMOD_CODEC_STATE *codec, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
    BLT_Result result;
    
    BLT_Codec* data = (BLT_Codec*)codec->plugindata;
    if (data->decoder == NULL) {
        fprintf(stderr, "ERROR: Invalid plugindata\n");
        return FMOD_ERR_INVALID_HANDLE;
    }
    
    result = BLT_Decoder_SeekToTime(data->decoder, position);
    if (BLT_FAILED(result)) {
        return FMOD_ERR_INVALID_POSITION;
    }
    
    return FMOD_OK;
}

FMOD_CODEC_DESCRIPTION bltcodec =
{
    "BlueTune codec",                   // Name.
    0x00010000,                         // Version 0xAAAABBBB   A = major, B = minor.
    1,                                  // Don't force everything using this codec to be a stream
    FMOD_TIMEUNIT_MS,                   // The time format we would like to accept into setposition/getposition.
    &bltopen,                           // Open callback.
    &bltclose,                          // Close callback.
    &bltread,                           // Read callback.
    0,                                  // Getlength callback.  (If not specified FMOD return the length in FMOD_TIMEUNIT_PCM, FMOD_TIMEUNIT_MS or FMOD_TIMEUNIT_PCMBYTES units based on the lengthpcm member of the FMOD_CODEC structure).
    &bltsetposition,                    // Setposition callback.
    0,                                  // Getposition callback. (only used for timeunit types that are not FMOD_TIMEUNIT_PCM, FMOD_TIMEUNIT_MS and FMOD_TIMEUNIT_PCMBYTES).
    0                                   // Sound create callback (don't need it)
};

/*
 FMODGetCodecDescription is mandatory for every fmod plugin.  This is the symbol the registerplugin function searches for.
 Must be declared with F_API to make it export as stdcall.
 MUST BE EXTERN'ED AS C!  C++ functions will be mangled incorrectly and not load in fmod.
 */
#ifdef __cplusplus
extern "C" {
#endif
	
	F_DECLSPEC F_DLLEXPORT FMOD_CODEC_DESCRIPTION * F_API FMODGetBltCodecDescription()
	{
		return &bltcodec;
	}
	
#ifdef __cplusplus
}
#endif
