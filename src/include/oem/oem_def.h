#pragma once

#define CONNECT_SUCCESS     0   ///< connected to probe
#define CONNECT_DISCONNECT  1   ///< disconnected from probe
#define CONNECT_FAILED      2   ///< failed to connect to probe
#define CONNECT_SWUPDATE    3   ///< software update required
#define CONNECT_ERROR       -1  ///< connection call error

#define CERT_INVALID        -1  ///< certificate is not valid

#define IMAGING_NOTREADY    0   ///< imaging is not ready, probe and application need to be loaded
#define IMAGING_READY       1   ///< imaging is ready
#define IMAGING_CERTEXPIRED 2   ///< cannot image due to expired certificate
#define IMAGING_POORWIFI    3   ///< stopped imaging due to poor wifi
#define IMAGING_NOCONTACT   4   ///< stopped imaging due to no patient contact detected
#define IMAGING_CHARGING    5   ///< probe started running or stopped due to change in charging status

#define POWERDOWN_IDLE      0   ///< probe was idle from imaging for extended period
#define POWERDOWN_TOOHOT    1   ///< probe got too hot
#define POWERDOWN_BATTERY   2   ///< low battery
#define POWERDOWN_BUTTON    3   ///< user held button to shut down probe

#define SWUPDATE_SUCCESS    0   ///< successful update
#define SWUPDATE_CURRENT    1   ///< software is current
#define SWUPDATE_BATTERY    2   ///< battery is too low to perform update
#define SWUPDATE_CORRUPT    3   ///< probe file system may be corrupt
#define SWUPDATE_ERROR      -1  ///< software update error

#define BUTTON_UP           0   ///< button up identifier
#define BUTTON_DOWN         1   ///< button down identifier

#define PARAM_DEPTH         0   ///< imaging depth in cm
#define PARAM_GAIN          1   ///< gain in percent
#define PARAM_DYNRNG        2   ///< dynamic range in percent
#define PARAM_AUTOGAIN      3   ///< auto gain enable
#define PARAM_CGAIN         4   ///< color/power gain in percent
#define PARAM_CPRF          5   ///< color/power pulse repetition frequency in kHz
#define PARAM_IMU           6   ///< imu stream enable
#define PARAM_SYNC          7   ///< sync pulse enable
#define PARAM_NESIDE        8   ///< needle enhance side

#define MODE_B              0   ///< b/grayscale imaging mode
#define MODE_RF             1   ///< rf capture mode (interleaved with b)
#define MODE_CFI            2   ///< color flow imaging mode
#define MODE_PDI            3   ///< power doppler imaging mode
#define MODE_SC             4   ///< spatial compounding mode (available in some workflows)
#define MODE_NEEDLE         5   ///< needle enhance mode (available on linear probes)

#define ROI_SIZE            0   ///< roi resizing function (adjusts bottom/right)
#define ROI_MOVE            1   ///< roi move function (adjusts top/left)

#define FORMAT_ARGB         0   ///< processed images are sent in a raw and uncompressed in 32 bit argb
#define FORMAT_JPEG         1   ///< processed images are sent as a jpeg
#define FORMAT_PNG          2   ///< processed images are sent as a png

#define BUTTON_FREEZE       0   ///< button setting to freeze imaging on probe
#define BUTTON_USER         1   ///< button setting to send interrupt through api
#define BUTTON_DISABLED     2   ///< button setting to disable handling

/// tgc structure
typedef struct _ClariusTgc
{
    double top;     ///< top tgc in dB
    double mid;     ///< mid tgc in dB
    double bottom;  ///< bottom tgc in dB

} ClariusTgc;

/// raw image information supplied with each frame
typedef struct _ClariusRawImageInfo
{
    int lines;              ///< number of ultrasound lines in the image
    int samples;            ///< number of samples per line in the image
    int bitsPerSample;      ///< bits per sample
    double axialSize;       ///< axial microns per sample
    double lateralSize;     ///< lateral microns per line
    long long int tm;       ///< timestamp of image
    int jpeg;               ///< size of the jpeg image, 0 if not a jpeg compressed image
    int rf;                 ///< flag specifying data is rf and not envelope

} ClariusRawImageInfo;

/// processed image information supplied with each frame
typedef struct _ClariusProcessedImageInfo
{
    int width;              ///< width of the image in pixels
    int height;             ///< height of the image in pixels
    int bitsPerPixel;       ///< bits per pixel
    int imageSize;          ///< total size of image
    double micronsPerPixel; ///< microns per pixel (always 1:1 aspect ratio axially/laterally)
    double originX;         ///< image origin in microns in the horizontal axis
    double originY;         ///< image origin in microns in the vertical axis
    long long int tm;       ///< timestamp of images
    int overlay;            ///< flag that the image is an overlay without grayscale (ie. color doppler or strain)
    int format;             ///< flag specifying the format of the image (see format definitions above)

} ClariusProcessedImageInfo;

/// spectral image information supplied with each block
typedef struct _ClariusSpectralImageInfo
{
    int lines;                  ///< number of lines in the block
    int samples;                ///< number of samples per line
    int bitsPerSample;          ///< bits per sample
    double period;              ///< line acquisition period in seconds
    double micronsPerSample;    ///< microns per pixel/sample in an m spectrum
    double velocityPerSample;   ///< velocity in m/s per pixel/sample in a pw spectrum
    int pw;                     ///< flag specifying the data is pw and not m

} ClariusSpectralImageInfo;

/// status information
typedef struct _ClariusStatusInfo
{
    int battery;        ///< battery level in percent
    int temperature;    ///< temperature level in percent
    double frameRate;   ///< current imaging frame rate

} ClariusStatusInfo;

/// probe information
typedef struct _ClariusProbeInfo
{
    int version;    ///< version (1 = Clarius 1st Generation, 2 = Clarius HD)
    int elements;   ///< # of probe elements
    int pitch;      ///< element pitch
    int radius;     ///< radius in mm

} ClariusProbeInfo;

/// probe settings
typedef struct _ClariusProbeSettings
{
    int contactDetection;   ///< the number of seconds to enage a lower frame rate when no contact is detected. valid range is 0 - 30, where 0 turns the function off
    int autoFreeze;         ///< the number of seconds to enage freezing imaging after no contact mode has been engaged. valid range is 0 - 120, where 0 turns the function off
    int keepAwake;          ///< the number of minutes to power down the device once imaging has been frozen. valid range is 0 - 120, where 0 turns the function off
    int wifiOptimization;   ///< flag allowing the probe to automatically freeze when poor wifi connectivity is detected
    int buttonUp;           ///< button up setting (see button settings above)
    int buttonDown;         ///< button down setting (see button settings above)

} ClariusProbeSettings;

/// positional data information structure
typedef struct _ClariusPosInfo
{
    long long int tm;   ///< timestamp in nanoseconds
    double gx;          ///< gyroscope x; angular velocity is given in radians per second (rps)
    double gy;          ///< gyroscope y
    double gz;          ///< gyroscope z
    double ax;          ///< accelerometer x; acceleration is normalized to gravity [~9.81m/s^2] (m/s^2)/(m/s^2)
    double ay;          ///< accelerometer y
    double az;          ///< accelerometer z
    double mx;          ///< magnetometer x; magnetic flux density is normalized to the earth's field [~50 mT] (T/T)
    double my;          ///< magnetometer y
    double mz;          ///< magnetometer z
    double qw;          ///< w component (real) of the orientation quaternion
    double qx;          ///< x component (imaginary) of the orientation quaternion
    double qy;          ///< y component (imaginary) of the orientation quaternion
    double qz;          ///< z component (imaginary) of the orientation quaternion

} ClariusPosInfo;

/// string list callback function
/// @param[in] list the string list
/// @param[in] sz the size of the string buffer
typedef void (*ClariusListFn)(const char* list, int sz);
/// connection callback function
/// @param[in] ret the return code, see CONNECT_ definitions above
/// @param[in] port udp port used for streaming
/// @param[in] status the status message
typedef void (*ClariusConnectFn)(int ret, int port, const char* status);
/// certification callback function
/// @param[in] daysValid # of days valid for certificate
typedef void (*ClariusCertFn)(int daysValid);
/// powering down callback function
/// @param[in] ret the return code, see POWERDOWN_ definitions above
/// @param[in] tm time for when probe is powering down, 0 for immediately
typedef void (*ClariusPowerDownFn)(int ret, int tm);
/// software update callback function
/// @param[in] ret the return code, see SWUPDATE_ definitions above
typedef void (*ClariusSwUpdateFn)(int ret);
/// new data callback function
/// @param[in] newImage pointer to the new grayscale image information
/// @param[in] nfo image information associated with the image data
/// @param[in] npos number of positional information data tagged with the image
/// @param[in] pos the positional information data tagged with the image
typedef void (*ClariusNewRawImageFn)(const void* newImage, const ClariusRawImageInfo* nfo, int npos, const ClariusPosInfo* pos);
/// new image callback function
/// @param[in] newImage pointer to the new grayscale image information
/// @param[in] nfo image information associated with the image data
/// @param[in] npos number of positional information data tagged with the image
/// @param[in] pos the positional information data tagged with the image
typedef void (*ClariusNewProcessedImageFn)(const void* newImage, const ClariusProcessedImageInfo* nfo, int npos, const ClariusPosInfo* pos);
/// new spectral image callback function
/// @param[in] newImage pointer to the new grayscale image information
/// @param[in] nfo image information associated with the image data
typedef void (*ClariusNewSpectralImageFn)(const void* newImage, const ClariusSpectralImageInfo* nfo);
/// imaging callback function
/// @param[in] ready the ready code, see IMAGING_ defintions above
/// @param[in] imaging 1 = running , 0 = stopped
typedef void (*ClariusImagingFn)(int ready, int imaging);
/// button callback function
/// @param[in] btn see BUTTON_ definitions above
/// @param[in] clicks # of clicks performed
typedef void (*ClariusButtonFn)(int btn, int clicks);
/// progress callback function
/// @param[in] progress the current progress in percent
typedef void (*ClariusProgressFn)(int progress);
/// error callback function
/// @param[in] msg the error message with associated error that occurred
typedef void (*ClariusErrorFn)(const char* msg);
