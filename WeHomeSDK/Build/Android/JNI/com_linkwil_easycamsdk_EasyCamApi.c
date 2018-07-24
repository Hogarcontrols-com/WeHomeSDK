/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "ECLog.h"
#include "EasyCamSdk.h"
#include "JNIEnv.h"
#include "YUV2RGB.h"

#define MAX_VIDEO_FRAME_SIZE        (512*1024)
#define MAX_AUDIO_FRAME_SIZE        (10*1024)
#define MAX_FILE_DOWNLOAD_FRAME_SIZE (10*1024)

extern void initAVFrameJByteArray(void);
extern void* getVideoFrameJByteArray(int handle);
extern void setVideoFrameJByteArray(int handle, void* byteArray);
extern void deleteVideoFrameJByteArray(int handle);
extern void* getAudioFrameJByteArray(int handle);
extern void setAudioFrameJByteArray(int handle, void* byteArray);
extern void deleteAudioFrameJByteArray(int handle);
extern void* getPBVideoFrameJByteArray(int handle);
extern void setPBVideoFrameJByteArray(int handle, void* byteArray);
extern void deletePBVideoFrameJByteArray(int handle);
extern void* getPBAudioFrameJByteArray(int handle);
extern void setPBAudioFrameJByteArray(int handle, void* byteArray);
extern void deletePBAudioFrameJByteArray(int handle);

extern void* getFileDownloadJByteArray(int handle);
extern void setFileDownloadJByteArray(int handle, void* byteArray);
extern void deleteFileDownloadJByteArray(int handle);

JavaVM* g_JVM = 0;
jclass g_CallbackCalss;
jmethodID g_methodID_OnLogInResult;
jmethodID g_methodID_OnCmdResult;
jmethodID g_methodID_OnAudio_RecvData;
jmethodID g_methodID_OnVideo_RecvData;
jmethodID g_methodID_OnPBVideo_RecvData;
jmethodID g_methodID_OnPBAudio_RecvData;
jmethodID g_methodID_OnPBEnd;
jmethodID g_methodId_OnFileDownload_RecvData;
jmethodID g_methodId_OnPIRData_RecvData;
jmethodID g_methodID_OnlineQueryResult;

static void onLoginResult(int handle, int errorCode, int seq, unsigned int notificationToken, unsigned int isCharging, unsigned int batPercent)
{
	LOGD("onLoginResult, handle:%d, errorCode:%d, seq:%d", handle, errorCode, seq);

	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
	//LOGD("onLoginResult env:%p, pid:%#X", env, pthread_self());
	if( env != NULL )
	{
	    //如果设备连接成功，会返回消息订阅的令牌，该令牌用于确保
        //消息订阅的安全性，设备端如果复位或者修改系统密码，该令牌
        //将不再有效,需要重新获取并重新订阅消息

	    (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnLogInResult,
	        handle, errorCode, seq, notificationToken, isCharging, batPercent);
	}
}

static void onCmdResult(int handle, char* data, int errorCode, int seq)
{
	LOGD("onCmdResult, handle:%d, seq:%d, data:%s", handle, seq, data);
	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray commandByteArray = (*env)->NewByteArray(env, strlen(data));
        if (commandByteArray != NULL) {
            (*env)->SetByteArrayRegion(env, commandByteArray, 0, strlen(data), (jbyte* )data);
        }
        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnCmdResult,
            handle, errorCode, commandByteArray, seq);
    }
}

static void OnAudio_RecvData(int handle, char *data, int len, int payloadType,
		long long timestamp, int seq)
{
	//LOGD("OnAudio_RecvData, handle:%d, len:%d", handle, len);

    if( len > MAX_AUDIO_FRAME_SIZE )
    {
        LOGE("Drop audio frame too large, size:%d", len);
        return;
    }

	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray audioFrameArray = getAudioFrameJByteArray(handle);
        if( audioFrameArray == NULL )
        {
            LOGD("Alloc jbyteArry for audio frame");
            audioFrameArray = (*env)->NewGlobalRef( env,
                                   (*env)->NewByteArray(env,MAX_AUDIO_FRAME_SIZE) );
            setAudioFrameJByteArray( handle, audioFrameArray );
            LOGD("Alloc jbyteArry for audio frame complete");
        }

        (*env)->SetByteArrayRegion( env, audioFrameArray, 0, len, (jbyte *)data );

        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnAudio_RecvData,
            handle, audioFrameArray, len, payloadType, timestamp, seq);
    }
}

static void OnVideo_RecvData(int handle, char *data, int len, int payloadType,
		long long timestamp, int seq, int frameType, int videoWidth, int videoHeight, unsigned int wifiQuality)
{
	//LOGD("OnVideo_RecvData, handle:%d, len:%d, timestamp:%u", handle, len, timestamp);
	//LOGD("[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]",
	//    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] );
	if( len > MAX_VIDEO_FRAME_SIZE )
	{
	    LOGE("Drop video frame too large, size:%d", len);
	    return;
	}

	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray videoFrameArray = getVideoFrameJByteArray(handle);
        if( videoFrameArray == NULL )
        {
            LOGD("Alloc jbyteArry for video frame");
            videoFrameArray = (*env)->NewGlobalRef( env,
                                 (*env)->NewByteArray(env,MAX_VIDEO_FRAME_SIZE) );
            setVideoFrameJByteArray( handle, videoFrameArray );
            LOGD("Alloc jbyteArry for video frame complete");
        }

        (*env)->SetByteArrayRegion( env, videoFrameArray, 0, len, (jbyte *)data );

        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnVideo_RecvData,
            handle, videoFrameArray, len, payloadType, timestamp, seq, frameType, videoWidth, videoHeight, wifiQuality);
    }
}

static void OnPBAudio_RecvData(int handle, char *data, int len, int payloadType,
		long long timestamp, int seq, int pbSessionNo)
{
    if( len > MAX_AUDIO_FRAME_SIZE )
    {
        LOGE("Drop pb audio frame too large, size:%d", len);
        return;
    }

	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray audioFrameArray = getPBAudioFrameJByteArray(handle);
        if( audioFrameArray == NULL )
        {
            LOGD("Alloc jbyteArry for pb audio frame");
            audioFrameArray = (*env)->NewGlobalRef( env,
                                   (*env)->NewByteArray(env,MAX_AUDIO_FRAME_SIZE) );
            setPBAudioFrameJByteArray( handle, audioFrameArray );
            LOGD("Alloc jbyteArry for pb audio complete");
        }

        (*env)->SetByteArrayRegion( env, audioFrameArray, 0, len, (jbyte *)data );

        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnPBAudio_RecvData,
            handle, audioFrameArray, len, payloadType, timestamp, seq, pbSessionNo);
    }
}

static void OnPBVideo_RecvData(int handle, char *data, int len, int payloadType,
		long long timestamp, int seq, int frameType, int videoWidth, int videoHeight, int pbSessionNo)
{
    //LOGD("OnPBVideo_RecvData");
    if( len > MAX_VIDEO_FRAME_SIZE )
	{
	    LOGE("Drop video frame too large, size:%d", len);
	    return;
	}

	JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray videoFrameArray = getPBVideoFrameJByteArray(handle);
        if( videoFrameArray == NULL )
        {
            LOGD("Alloc jbyteArry for pb video frame");
            videoFrameArray = (*env)->NewGlobalRef( env,
                                 (*env)->NewByteArray(env,MAX_VIDEO_FRAME_SIZE) );
            setPBVideoFrameJByteArray( handle, videoFrameArray );
        }

        (*env)->SetByteArrayRegion( env, videoFrameArray, 0, len, (jbyte *)data );

        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnPBVideo_RecvData,
            handle, videoFrameArray, len, payloadType, timestamp, seq, frameType, videoWidth, videoHeight, pbSessionNo);
    }
}

static void OnPBEnd(int handle, int pbSessionNo)
{
    JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnPBEnd, handle, pbSessionNo);
    }
}

static void OnFileDownload_RecvData(int handle, char* data, int dataLen, int sessionNo)
{
    JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        jbyteArray fileDownloadFrameArray = getFileDownloadJByteArray(handle);
        if( fileDownloadFrameArray == NULL )
        {
            LOGD("Alloc jbyteArry for file download frame");
            fileDownloadFrameArray = (*env)->NewGlobalRef( env,
                                                    (*env)->NewByteArray(env,MAX_FILE_DOWNLOAD_FRAME_SIZE) );
            setFileDownloadJByteArray( handle, fileDownloadFrameArray );
            LOGD("Alloc jbyteArry for file download complete");
        }

        (*env)->SetByteArrayRegion( env, fileDownloadFrameArray, 0, dataLen, (jbyte *)data );

        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodId_OnFileDownload_RecvData,
                                     handle, fileDownloadFrameArray, dataLen, sessionNo);
    }
}

static void OnPIRData_RecvData(int handle, long long timeMS, short adc)
{
    JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
    if( env != NULL )
    {
        (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodId_OnPIRData_RecvData,
                                     handle, (int)timeMS, (int)adc);
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("[EC_JNI] JNI_OnLoad()");

    initAVFrameJByteArray();

	EC_INIT_INFO initInfo;
	initInfo.lpLoginResult = onLoginResult;
	initInfo.lpCmdResult = onCmdResult;
	initInfo.lpAudio_RecvData = OnAudio_RecvData;
	initInfo.lpVideo_RecvData = OnVideo_RecvData;
	initInfo.lpPBAudio_RecvData = OnPBAudio_RecvData;
	initInfo.lpPBVideo_RecvData = OnPBVideo_RecvData;
	initInfo.lpPBEnd = OnPBEnd;
    initInfo.lpFileDownload_RecvData = OnFileDownload_RecvData;
    initInfo.lpPIRData_RecvData = OnPIRData_RecvData;
    EC_Initialize( &initInfo );

    return JNI_VERSION_1_6;
}

JNIEXPORT jstring JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_getSdkVersion
  (JNIEnv * env, jobject thiz)
{
	LOGD("[EC_JNI] getSdkVersion");
	char version[64] = {0};

	EC_GetVersion(version);
	jstring sdkVersionStr = (*env)->NewStringUTF(env, version);

	return sdkVersionStr;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_init
  (JNIEnv * env, jobject thiz)
{
    if( g_JVM == 0 )
    {
        LOGD("Register JVM env");
        (*env)->GetJavaVM( env, &g_JVM );
        jclass cls = (*env)->FindClass( env, "com/linkwil/easycamsdk/EasyCamApi" );
        g_CallbackCalss = (*env)->NewGlobalRef( env, cls );
        g_methodID_OnLogInResult = (*env)->GetStaticMethodID( env, cls, "onLoginResult", "(IIIIII)V" );
        g_methodID_OnCmdResult = (*env)->GetStaticMethodID( env, cls, "onCmdResult", "(II[BI)V" );
        g_methodID_OnAudio_RecvData = (*env)->GetStaticMethodID( env, cls, "OnAudio_RecvData", "(I[BIIJI)V" );
        g_methodID_OnVideo_RecvData = (*env)->GetStaticMethodID( env, cls, "OnVideo_RecvData", "(I[BIIJIIIII)V" );
        g_methodID_OnPBAudio_RecvData = (*env)->GetStaticMethodID( env, cls, "OnPBAudio_RecvData", "(I[BIIJII)V" );
        g_methodID_OnPBVideo_RecvData = (*env)->GetStaticMethodID( env, cls, "OnPBVideo_RecvData", "(I[BIIJIIIII)V" );
        g_methodID_OnPBEnd = (*env)->GetStaticMethodID( env, cls, "OnPBEnd", "(II)V" );
        g_methodId_OnFileDownload_RecvData = (*env)->GetStaticMethodID( env, cls, "OnFileDownload_RecvData", "(I[BII)V" );
        g_methodID_OnlineQueryResult = (*env)->GetStaticMethodID( env, cls, "OnlineStatusQueryResult", "(Ljava/lang/String;II)V" );
        g_methodId_OnPIRData_RecvData = (*env)->GetStaticMethodID( env, cls, "OnPIRData", "(III)V" );

#if 0
        LOGD("g_methodID_OnLogInResult:%p", g_methodID_OnLogInResult);
        LOGD("g_methodID_OnCmdResult:%p", g_methodID_OnCmdResult);
        LOGD("g_methodID_OnAudio_RecvData:%p", g_methodID_OnAudio_RecvData);
        LOGD("g_methodID_OnVideo_RecvData:%p", g_methodID_OnVideo_RecvData);
        LOGD("g_methodID_OnlineQueryResult:%p", g_methodID_OnlineQueryResult);
#endif
    }

    return 0;
}

void ECOnlineQueryResultCallback(int queryResult, const char* uid, int lastLoginSec)
{
    LOGI("uid:%s queryResult:%d, lastLogin:%d", uid, queryResult, lastLoginSec);

    // 回掉给JNI层
    if( g_methodID_OnlineQueryResult != NULL )
    {
        JNIEnv* env = (JNIEnv* )getJNIEnv(pthread_self());
        if( env != NULL )
        {
            (*env)->CallStaticVoidMethod(env, g_CallbackCalss, g_methodID_OnlineQueryResult,
                        (*env)->NewStringUTF(env,uid), queryResult, lastLoginSec);
        }
    }
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_onlineStatusQuery
  (JNIEnv * env, jobject thiz, jstring uid)
{
    const char *str = NULL;
    char uidTemp[64] = {0};

    str = (*env)->GetStringUTFChars(env, uid, 0);
    strcpy( uidTemp, str );
    (*env)->ReleaseStringUTFChars(env, uid, str);

    return EC_QueryOnlineStatus(uidTemp, ECOnlineQueryResultCallback);
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_logIn
  (JNIEnv * env, jobject thiz, jstring uid, jstring password, jstring bCastAddr,
  jint seq, jint needVideo, jint needAudio, jint connectType, jint timeoutS)
{
    LOGD("[EC_JNI] logIn");

    int handle = 0;
    const char *str = NULL;
    char uidTemp[64] = {0};
    char passwordTemp[64] = {0};
    char bcastAddrTemp[64] = {0};

    str = (*env)->GetStringUTFChars(env, uid, 0);
    strcpy( uidTemp, str );
    (*env)->ReleaseStringUTFChars(env, uid, str);

    str = (*env)->GetStringUTFChars(env, password, 0);
    strcpy( passwordTemp, str );
    (*env)->ReleaseStringUTFChars(env, password, str);

    str = (*env)->GetStringUTFChars(env, bCastAddr, 0);
    strcpy( bcastAddrTemp, str );
    (*env)->ReleaseStringUTFChars(env, bCastAddr, str);

    handle = EC_Login(uidTemp, "admin", passwordTemp, bcastAddrTemp, seq,
        needVideo, needAudio, connectType, timeoutS);

    return handle;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_logOut
  (JNIEnv * env, jobject thiz, jint handle)
{
    LOGD("[EC_JNI] logOut");

    int ret = 0;

    ret = EC_Logout(handle);

    jbyteArray videoFrameArray = getVideoFrameJByteArray(handle);
    jbyteArray audioFrameArray = getAudioFrameJByteArray(handle);
    jbyteArray pbVideoFrameArray = getPBVideoFrameJByteArray(handle);
    jbyteArray pbAudioFrameArray = getPBAudioFrameJByteArray(handle);
    jbyteArray fileDownloadFrameArray = getFileDownloadJByteArray(handle);

    if( videoFrameArray != NULL )
    {
        (*env)->DeleteGlobalRef( env, videoFrameArray );
        deleteVideoFrameJByteArray(handle);
    }

    if( audioFrameArray != NULL )
    {
        (*env)->DeleteGlobalRef( env, audioFrameArray );
        deleteAudioFrameJByteArray(handle);
    }

    if( pbVideoFrameArray != NULL )
    {
        (*env)->DeleteGlobalRef( env, pbVideoFrameArray );
        deletePBVideoFrameJByteArray(handle);
    }

    if( pbAudioFrameArray != NULL )
    {
        (*env)->DeleteGlobalRef( env, pbAudioFrameArray );
        deletePBAudioFrameJByteArray(handle);
    }

    if( fileDownloadFrameArray != NULL )
    {
        (*env)->DeleteGlobalRef(env, fileDownloadFrameArray);
        deleteFileDownloadJByteArray(handle);
    }

    return ret;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_startSearchDev
  (JNIEnv * env, jobject thiz, jint timeout, jstring bCastAddr)
{
    LOGD("[EC_JNI] startSearchDev");

    char bCastAddrTmp[64] = {0};
    if( bCastAddr != NULL )
    {
        const char* str = (*env)->GetStringUTFChars(env, bCastAddr, 0);
        strcpy( bCastAddrTmp, str );
        (*env)->ReleaseStringUTFChars(env, bCastAddr, str);
    }

    return EC_StartSearchDev(timeout, bCastAddrTmp);
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_stopSearchDev
  (JNIEnv * env, jobject thiz)
{
    LOGD("[EC_JNI] stopSearchDev");
    return EC_StopDevSearch();
}

jobjectArray JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_getDevList
   (JNIEnv * env, jobject thiz)
{
    jobjectArray devList = NULL;
	jmethodID mid;

	jclass devInfoClass = (*env)->FindClass(env, "com/linkwil/easycamsdk/EasyCamApi$DevListInfo");


    int devCnt = 0;
    DeviceInfo dev[100];
    memset( &dev, 0x00, sizeof(dev) );
    devCnt = EC_GetDevList(dev, 100);
    if( devCnt > 0 )
    {
        devList = (*env)->NewObjectArray( env, devCnt, devInfoClass, 0 );

		jfieldID fieldIDDevType = (*env)->GetFieldID( env, devInfoClass, "devType", "I" );
		jfieldID fieldIDDevName = (*env)->GetFieldID( env, devInfoClass, "devName", "Ljava/lang/String;" );
		jfieldID fieldIDUID = (*env)->GetFieldID( env, devInfoClass, "uid", "Ljava/lang/String;" );
		jfieldID fieldIDVer = (*env)->GetFieldID( env, devInfoClass, "fwVer", "Ljava/lang/String;" );

        int i = 0;
        for( i = 0; i < devCnt; i++ )
        {
            mid = (*env)->GetMethodID(env, devInfoClass, "<init>", "()V");
            jobject newObj = (*env)->NewObject(env, devInfoClass, mid );
            (*env)->SetIntField( env, newObj, fieldIDDevType, dev[i].devType );
            (*env)->SetObjectField( env, newObj, fieldIDDevName, (*env)->NewStringUTF(env, dev[i].devName) );
            (*env)->SetObjectField( env, newObj, fieldIDUID, (*env)->NewStringUTF(env, dev[i].uid) );
            (*env)->SetObjectField( env, newObj, fieldIDVer, (*env)->NewStringUTF(env, dev[i].fwVer) );
            (*env)->SetObjectArrayElement( env, devList, i, newObj );
        }
    }

    return devList;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_sendCommand
  (JNIEnv * env, jobject thiz, jint handle, jstring command, jint len, jint seq)
{
    LOGD("[EC_JNI] sendCommand");

    int ret = 0;

    char *commandTemp = (char* )malloc(len+1);
    const char* str = NULL;

    memset( commandTemp, 0x00, len+1 );
    str = (*env)->GetStringUTFChars(env, command, 0);
    strcpy( commandTemp, str );
    (*env)->ReleaseStringUTFChars(env, command, str);

    ret = EC_SendCommand(handle, commandTemp, seq);

    free(commandTemp);

    return ret;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_sendTalkData
  (JNIEnv * env, jobject thiz, jint handle, jbyteArray data, jint len, jint payload, jint seq)
{
    //LOGD("[EC_JNI] sendTalkData");
    int ret = 0;

    jbyte* arrayBody = (*env)->GetByteArrayElements( env, data, 0);
    ret = EC_SendTalkData(handle, (char* )arrayBody, len, PAYLOAD_TYPE_AUDIO_PCM, seq);
    (*env)->ReleaseByteArrayElements( env, data, arrayBody, 0 );

    return ret;
}

JNIEXPORT jint JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_networkDetect
        (JNIEnv * env, jobject thiz, jstring uid)
{
    LOGD("[EC_JNI] networkDetect");

    int ret = 0;

    char uidTmp[32] = {0};
    const char* str = NULL;

    str = (*env)->GetStringUTFChars(env, uid, 0);
    strcpy( uidTmp, str );
    (*env)->ReleaseStringUTFChars(env, uid, str);

    ret = EC_Detect_Network_Type(uidTmp);

    return ret;
}

JNIEXPORT jintArray JNICALL Java_com_linkwil_easycamsdk_EasyCamApi_YUV420P2ARGB8888
   (JNIEnv * env, jobject thiz, jbyteArray buf, jint width, jint height)
{
    jbyte * yuv420sp = (*env)->GetByteArrayElements(env, buf, 0);
    int frameSize = width * height;
    jint *rgb = (jint* )malloc(frameSize); // 新图像像素值
    YUV420P2ARGB8888((char* )yuv420sp, width, height, (char* )rgb);
    (*env)->ReleaseByteArrayElements(env, buf, yuv420sp, 0);

    jintArray result = (*env)->NewIntArray(env, frameSize);
    (*env)->SetIntArrayRegion(env, result, 0, frameSize, rgb);

    free( rgb );

    return result;
}

// EOF



JNIEXPORT jint JNICALL
Java_com_linkwil_easycamsdk_EasyCamApi_subscribeMessage(JNIEnv *env, jobject instance, jstring uid_,
                                                        jstring appName_, jstring agName_,
                                                        jstring phoneToken_, jint eventCh) {
    int ret = 0;

    const char *uid = (*env)->GetStringUTFChars(env, uid_, 0);
    const char *appName = (*env)->GetStringUTFChars(env, appName_, 0);
    const char *agName = (*env)->GetStringUTFChars(env, agName_, 0);
    const char *phoneToken = (*env)->GetStringUTFChars(env, phoneToken_, 0);

    ret = EC_Subscribe(uid, appName, agName, phoneToken, (unsigned int)eventCh);
    LOGD("EC_Subscribe, ret:%d", ret);

    (*env)->ReleaseStringUTFChars(env, uid_, uid);
    (*env)->ReleaseStringUTFChars(env, appName_, appName);
    (*env)->ReleaseStringUTFChars(env, agName_, agName);
    (*env)->ReleaseStringUTFChars(env, phoneToken_, phoneToken);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_linkwil_easycamsdk_EasyCamApi_unSubscribe(JNIEnv *env, jobject instance, jstring uid_,
                                                   jstring appName_, jstring agName_,
                                                   jstring phoneToken_, jint eventCh) {

    int ret = 0;

    const char *uid = (*env)->GetStringUTFChars(env, uid_, 0);
    const char *appName = (*env)->GetStringUTFChars(env, appName_, 0);
    const char *agName = (*env)->GetStringUTFChars(env, agName_, 0);
    const char *phoneToken = (*env)->GetStringUTFChars(env, phoneToken_, 0);
    ret = EC_UnSubscribe(uid, appName, agName, phoneToken, (unsigned int)eventCh);
    (*env)->ReleaseStringUTFChars(env, uid_, uid);
    (*env)->ReleaseStringUTFChars(env, appName_, appName);
    (*env)->ReleaseStringUTFChars(env, agName_, agName);
    (*env)->ReleaseStringUTFChars(env, phoneToken_, phoneToken);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_linkwil_easycamsdk_EasyCamApi_resetBadge(JNIEnv *env, jobject instance, jstring uid_,
                                                  jstring appName_, jstring agName_,
                                                  jstring phoneToken_, jint eventCh) {
    int ret = 0;

    const char *uid = (*env)->GetStringUTFChars(env, uid_, 0);
    const char *appName = (*env)->GetStringUTFChars(env, appName_, 0);
    const char *agName = (*env)->GetStringUTFChars(env, agName_, 0);
    const char *phoneToken = (*env)->GetStringUTFChars(env, phoneToken_, 0);

    ret = EC_ResetBadge(uid, appName, agName, phoneToken, (unsigned int)eventCh);

    (*env)->ReleaseStringUTFChars(env, uid_, uid);
    (*env)->ReleaseStringUTFChars(env, appName_, appName);
    (*env)->ReleaseStringUTFChars(env, agName_, agName);
    (*env)->ReleaseStringUTFChars(env, phoneToken_, phoneToken);

    return ret;
}