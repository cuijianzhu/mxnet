#include <iostream>
#include <cstdlib>
#include <functional>
#include <mxnet/ndarray.h>
#include <mxnet/kvstore.h>
#include <mxnet/c_api.h>

#include "jni_helper_func.h"
#include "ml_dmlc_mxnet_native_c_api.h" // generated by javah

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayCreateNone(JNIEnv *env, jobject obj, jobject ndArrayHandle) {
  NDArrayHandle out;
  int ret = MXNDArrayCreateNone(&out);
  jclass ndClass = env->GetObjectClass(ndArrayHandle);
  jfieldID ptr = env->GetFieldID(ndClass, "value", "J");
  env->SetLongField(ndArrayHandle, ptr, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayCreate(JNIEnv *env, jobject obj,
                                                              jintArray shape,
                                                              jint ndim,
                                                              jint devType,
                                                              jint devId,
                                                              jint delayAlloc,
                                                              jobject ndArrayHandle) {
  jint *shapeArr = env->GetIntArrayElements(shape, NULL);
  NDArrayHandle out;
  int ret = MXNDArrayCreate((mx_uint *)shapeArr, (mx_uint)ndim, devType, devId, delayAlloc, &out);
  env->ReleaseIntArrayElements(shape, shapeArr, 0);
  jclass ndClass = env->GetObjectClass(ndArrayHandle);
  jfieldID ptr = env->GetFieldID(ndClass, "value", "J");
  env->SetLongField(ndArrayHandle, ptr, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayWaitAll(JNIEnv *env, jobject obj) {
  return MXNDArrayWaitAll();
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxListFunctions(JNIEnv *env, jobject obj, jobject functions) {
  // Base.FunctionHandle.constructor
  jclass fhClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jmethodID fhConstructor = env->GetMethodID(fhClass,"<init>","(J)V");

  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  // Get function list
  FunctionHandle *outArray;
  mx_uint outSize;
  int ret = MXListFunctions(&outSize, &outArray);
  for (int i = 0; i < outSize; ++i) {
    FunctionHandle fhAddr = outArray[i];
    jobject fhObj = env->NewObject(fhClass, fhConstructor, (long)fhAddr);
    env->CallObjectMethod(functions, listAppend, fhObj);
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncDescribe(JNIEnv *env, jobject obj,
                                                              jobject funcHandle,
                                                              jobject nUsedVars,
                                                              jobject nScalars,
                                                              jobject nMutateVars,
                                                              jobject typeMask) {
  jlong funcPtr = getLongField(env, funcHandle);

  mx_uint numUseVars;
  mx_uint numScalars;
  mx_uint numMutateVars;
  int type;
  int ret = MXFuncDescribe((FunctionHandle)funcPtr, &numUseVars,
                            &numScalars, &numMutateVars, &type);

  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID value = env->GetFieldID(refIntClass, "value", "I");
  env->SetIntField(nUsedVars, value, (jint)numUseVars);
  env->SetIntField(nScalars, value, (jint)numScalars);
  env->SetIntField(nMutateVars, value, (jint)numMutateVars);
  env->SetIntField(typeMask, value, (jint)type);

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncGetInfo(JNIEnv *env, jobject obj,
                                                              jobject funcHandle,
                                                              jobject name,
                                                              jobject desc,
                                                              jobject numArgs,
                                                              jobject argNames,
                                                              jobject argTypes,
                                                              jobject argDescs) {
  jlong funcPtr = getLongField(env, funcHandle);

  const char *cName;
  const char *cDesc;
  mx_uint cNumArgs;
  const char **cArgNames;
  const char **cArgTypes;
  const char **cArgDescs;
  int ret = MXFuncGetInfo((FunctionHandle)funcPtr,
                          &cName, &cDesc, &cNumArgs,
                          &cArgNames, &cArgTypes, &cArgDescs);

  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID valueInt = env->GetFieldID(refIntClass, "value", "I");

  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");

  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass, "$plus$eq",
      "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  env->SetObjectField(name, valueStr, env->NewStringUTF(cName));
  env->SetObjectField(desc, valueStr, env->NewStringUTF(cDesc));
  env->SetIntField(numArgs, valueInt, (jint)cNumArgs);
  for (int i = 0; i < cNumArgs; ++i) {
    env->CallObjectMethod(argNames, listAppend, env->NewStringUTF(cArgNames[i]));
    env->CallObjectMethod(argTypes, listAppend, env->NewStringUTF(cArgTypes[i]));
    env->CallObjectMethod(argDescs, listAppend, env->NewStringUTF(cArgDescs[i]));
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncInvoke(JNIEnv *env, jobject obj,
                                                            jobject funcHandle,
                                                            jlongArray useVars,
                                                            jfloatArray scalarArgs,
                                                            jlongArray mutateVars) {
  jlong funcPtr = getLongField(env, funcHandle);

  jlong *cUseVars = env->GetLongArrayElements(useVars, NULL);
  jfloat *cScalarArgs = env->GetFloatArrayElements(scalarArgs, NULL);
  jlong *cMutateVars = env->GetLongArrayElements(mutateVars, NULL);
  int ret = MXFuncInvoke((FunctionHandle)funcPtr,
                         (NDArrayHandle *)cUseVars,
                         (mx_float *)cScalarArgs,
                         (NDArrayHandle *)cMutateVars);
  env->ReleaseLongArrayElements(useVars, cUseVars, 0);
  env->ReleaseFloatArrayElements(scalarArgs, cScalarArgs, 0);
  env->ReleaseLongArrayElements(mutateVars, cMutateVars, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayGetShape(JNIEnv *env, jobject obj,
                                                                jobject ndArrayHandle,
                                                                jobject ndimRef,
                                                                jobject dataBuf) {
  jlong ndArrayPtr = getLongField(env, ndArrayHandle);

  mx_uint ndim;
  const mx_uint *pdata;
  int ret = MXNDArrayGetShape((NDArrayHandle)ndArrayPtr, &ndim, &pdata);

  // fill dataBuf
  jclass integerClass = env->FindClass("java/lang/Integer");
  jmethodID newInteger = env->GetMethodID(integerClass, "<init>", "(I)V");

  jclass arrayClass = env->FindClass("scala/collection/mutable/ArrayBuffer");
  jmethodID arrayAppend = env->GetMethodID(arrayClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ArrayBuffer;");
  for (int i = 0; i < ndim; ++i) {
    jobject data = env->NewObject(integerClass, newInteger, pdata[i]);
    env->CallObjectMethod(dataBuf, arrayAppend, data);
  }

  // set ndimRef
  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID valueInt = env->GetFieldID(refIntClass, "value", "I");
  env->SetIntField(ndimRef, valueInt, ndim);

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySyncCopyToCPU(JNIEnv *env, jobject obj,
                                                                      jobject ndArrayHandle,
                                                                      jfloatArray data,
                                                                      jint size) {
  jlong ndArrayPtr = getLongField(env, ndArrayHandle);

  jfloat *pdata = env->GetFloatArrayElements(data, NULL);
  int ret = MXNDArraySyncCopyToCPU((NDArrayHandle)ndArrayPtr, (mx_float *)pdata, size);
  env->ReleaseFloatArrayElements(data, pdata, 0); // copy back to java array automatically
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySlice(JNIEnv *env, jobject obj,
                                                                      jobject ndArrayHandle,
                                                                      jint start,
                                                                      jint end,
                                                                      jobject slicedHandle) {
  jclass refLongClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jfieldID refLongFid = env->GetFieldID(refLongClass, "value", "J");
  jlong ndArrayPtr = env->GetLongField(ndArrayHandle, refLongFid);
  NDArrayHandle out;
  int ret = MXNDArraySlice((NDArrayHandle)ndArrayPtr, start, end, &out);
  env->SetLongField(slicedHandle, refLongFid, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySyncCopyFromCPU
  (JNIEnv *env, jobject obj, jobject ndArrayHandle, jfloatArray sourceArr, jint arrSize) {
  jlong arrayPtr = getLongField(env, ndArrayHandle);
  jfloat *sourcePtr = env->GetFloatArrayElements(sourceArr, NULL);
  int ret = MXNDArraySyncCopyFromCPU((NDArrayHandle)arrayPtr, (const mx_float *)sourcePtr, arrSize);
  env->ReleaseFloatArrayElements(sourceArr, sourcePtr, 0);
  return ret;
}

// The related c api MXKVStoreSetUpdater function takes a c function pointer as its parameter,
// while we write java functions here in scala-package.
// Thus we have to wrap the function in a java object, and run env->CallVoidMethod(obj) once updater is invoked,
// which implies the function registered to KVStore must be stateful.
// This is why we re-implement MXKVStoreSetUpdater as follows.
JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreSetUpdater(JNIEnv *env, jobject obj,
                                                                      jobject kvStoreHandle,
                                                                      jobject updaterFuncObj,
                                                                      jobject updaterHandle) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jobject updaterFuncObjGlb = env->NewGlobalRef(updaterFuncObj);
  jobject updaterHandleGlb = env->NewGlobalRef(updaterHandle);
  std::function<void(int, const mxnet::NDArray&, mxnet::NDArray*)> updt
  = [env, updaterFuncObjGlb, updaterHandleGlb](int key, const mxnet::NDArray& recv, mxnet::NDArray* local) {
    // find java updater method
    jclass updtClass = env->GetObjectClass(updaterFuncObjGlb);
    jmethodID updtFunc = env->GetMethodID(updtClass,
      "update", "(ILml/dmlc/mxnet/NDArray;Lml/dmlc/mxnet/NDArray;Ljava/lang/Object;)V");

    // find java NDArray constructor
    jclass ndPtrClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
    jmethodID ndPtrConstructor = env->GetMethodID(ndPtrClass, "<init>","(J)V");
    jclass ndObjClass = env->FindClass("ml/dmlc/mxnet/NDArray");
    jmethodID ndObjConstructor = env->GetMethodID(ndObjClass, "<init>", "(Lml/dmlc/mxnet/Base$RefLong;Z)V");

    mxnet::NDArray *recvCopy = new mxnet::NDArray();
    *recvCopy = recv;
    jobject jNdRecvCopyPtr = env->NewObject(ndPtrClass, ndPtrConstructor, (long)recvCopy);
    jobject jNdRecvCopy = env->NewObject(ndObjClass, ndObjConstructor, jNdRecvCopyPtr, true);

    mxnet::NDArray *localCopy = new mxnet::NDArray();
    *localCopy = *local;
    jobject jNdLocalCopyPtr = env->NewObject(ndPtrClass, ndPtrConstructor, (long)localCopy);
    jobject jNdLocalCopy = env->NewObject(ndObjClass, ndObjConstructor, jNdLocalCopyPtr, true);

    env->CallVoidMethod(updaterFuncObjGlb, updtFunc, key, jNdRecvCopy, jNdLocalCopy, updaterHandleGlb);
    env->DeleteGlobalRef(updaterFuncObjGlb);
    env->DeleteGlobalRef(updaterHandleGlb);
  };
  try {
    static_cast<mxnet::KVStore*>((KVStoreHandle)kvStorePtr)->set_updater(updt);
  } catch(dmlc::Error &except) {
    // It'll be too complicated to set & get mx error in jni code.
    // thus simply return -1 to indicate a failure.
    // Notice that we'll NOT be able to run MXGetLastError
    // to get the error message after this function fails.
    return -1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreCreate(JNIEnv *env, jobject obj,
                                                                  jstring name,
                                                                  jobject kvStoreHandle) {
  jclass refLongClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jfieldID refLongFid = env->GetFieldID(refLongClass, "value", "J");

  KVStoreHandle out;
  const char *type = env->GetStringUTFChars(name, 0);
  int ret = MXKVStoreCreate(type, &out);
  env->ReleaseStringUTFChars(name, type);

  env->SetLongField(kvStoreHandle, refLongFid, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreInit(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray values) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *valueArray = env->GetLongArrayElements(values, NULL);
  int ret = MXKVStoreInit((KVStoreHandle) kvStorePtr,
                          (mx_uint) len,
                          (const int *) keyArray,
                          (NDArrayHandle *) valueArray);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(values, valueArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStorePush(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray values,
                                                                jint priority) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *valueArray = env->GetLongArrayElements(values, NULL);
  int ret = MXKVStorePush((KVStoreHandle)kvStorePtr,
                          (mx_uint)len,
                          (const int *)keyArray,
                          (NDArrayHandle *)valueArray,
                          priority);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(values, valueArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStorePull(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray outs,
                                                                jint priority) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *outArray = env->GetLongArrayElements(outs, NULL);
  int ret = MXKVStorePull((KVStoreHandle)kvStorePtr,
                          (mx_uint)len,
                          (const int *)keyArray,
                          (NDArrayHandle *)outArray,
                          priority);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(outs, outArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetType
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject kvType) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  const char *type;
  int ret = MXKVStoreGetType((KVStoreHandle)kvStorePtr, &type);
  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");
  env->SetObjectField(kvType, valueStr, env->NewStringUTF(type));
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreSendCommmandToServers
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jint head, jstring body) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  const char *bodyCStr = env->GetStringUTFChars(body, 0);
  int ret = MXKVStoreSendCommmandToServers((KVStoreHandle)kvStorePtr, head, bodyCStr);
  env->ReleaseStringUTFChars(body, bodyCStr);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreBarrier
  (JNIEnv *env, jobject obj, jobject kvStoreHandle) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  return MXKVStoreBarrier((KVStoreHandle)kvStorePtr);
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetGroupSize
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject sizeRef) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  int size;
  int ret = MXKVStoreGetGroupSize((KVStoreHandle)kvStorePtr, &size);
  setIntField(env, sizeRef, size);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetRank
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject rankRef) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  int rank;
  int ret = MXKVStoreGetRank((KVStoreHandle)kvStorePtr, &rank);
  setIntField(env, rankRef, rank);
  return ret;
}

JNIEXPORT jstring JNICALL Java_ml_dmlc_mxnet_LibInfo_mxGetLastError(JNIEnv * env, jobject obj) {
  char *tmpstr = "MXNetError";
  jstring rtstr = env->NewStringUTF(tmpstr);
  return rtstr;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayFree(JNIEnv * env, jobject obj, jobject ndArrayHandle) {
  // TODO
  puts("Free ndarray called");
  return 0;
}

//IO funcs
JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxListDataIters
  (JNIEnv * env, jobject obj, jobject creators) {
  // Base.FunctionHandle.constructor
    jclass chClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
    jmethodID chConstructor = env->GetMethodID(chClass,"<init>","(J)V");

    // scala.collection.mutable.ListBuffer append method
    jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
    jmethodID listAppend = env->GetMethodID(listClass,
      "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

    // Get function list
    DataIterCreator *outArray;
    mx_uint outSize;
    int ret = MXListDataIters(&outSize, &outArray);
    for (int i = 0; i < outSize; ++i) {
      DataIterCreator chAddr = outArray[i];
      jobject chObj = env->NewObject(chClass, chConstructor, (long)chAddr);
      env->CallObjectMethod(creators, listAppend, chObj);
    }
    return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDateIterCreateIter
  (JNIEnv * env, jobject obj, jobject creator,
   jobjectArray jkeys, jobjectArray jvals, jobject dataIterHandle) {
  //keys and values
  int paramSize = env->GetArrayLength(jkeys);
  char** keys = new char*[paramSize];
  char** vals = new char*[paramSize];
  jstring jkey, jval;
  for(int i=0; i<paramSize; i++) {
    jkey = (jstring) env->GetObjectArrayElement(jkeys, i);
    keys[i] = (char*)env->GetStringUTFChars(jkey, 0);
    jval = (jstring) env->GetObjectArrayElement(jvals, i);
    vals[i] = (char*)env->GetStringUTFChars(jval, 0);
  }

//  printf("paramSize: %d\n", paramSize);
//  for(int i=0; i<paramSize; i++) {
//    printf("key: %s\t",keys[i]);
//    printf("value: %s\n", vals[i]);
//  }

  //create iter
  jlong creatorPtr = getLongField(env, creator);
  DataIterHandle out;
  int ret = MXDataIterCreateIter((DataIterCreator)creatorPtr,
                                  (mx_uint) paramSize,
                                  (const char**) keys,
                                  (const char**) vals,
                                  &out);
  jclass hClass = env->GetObjectClass(dataIterHandle);
  jfieldID ptr = env->GetFieldID(hClass, "value", "J");
  env->SetLongField(dataIterHandle, ptr, (long)out);

  //release const char*
  for(int i=0; i<paramSize; i++) {
      jkey = (jstring) env->GetObjectArrayElement(jkeys, i);
      env->ReleaseStringUTFChars(jkey,(const char*)keys[i]);
      jval = (jstring) env->GetObjectArrayElement(jvals, i);
      env->ReleaseStringUTFChars(jval,(const char*)vals[i]);
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetIterInfo
  (JNIEnv * env, jobject obj, jobject creator, jobject jname,
   jobject jdesc, jobject jargNames, jobject jargTypeInfos, jobject jargDescs) {
  jlong creatorPtr = getLongField(env, creator);
  const char* name;
  const char* description;
  mx_uint numArgs;
  const char** argNames;
  const char** argTypeInfos;
  const char** argDescs;
  int ret = MXDataIterGetIterInfo((DataIterCreator)creatorPtr,
                                   &name,
                                   &description,
                                   &numArgs,
                                   &argNames,
                                   &argTypeInfos,
                                   &argDescs);

  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");
  //set params
  env->SetObjectField(jname, valueStr, env->NewStringUTF(name));
  env->SetObjectField(jdesc, valueStr, env->NewStringUTF(description));
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");
  for(int i=0; i<numArgs; i++) {
    env->CallObjectMethod(jargNames, listAppend, env->NewStringUTF(argNames[i]));
    env->CallObjectMethod(jargTypeInfos, listAppend, env->NewStringUTF(argTypeInfos[i]));
    env->CallObjectMethod(jargDescs, listAppend, env->NewStringUTF(argDescs[i]));
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterFree
  (JNIEnv *env, jobject obj, jobject handle) {
  jlong handlePtr = getLongField(env, handle);
  int ret = MXDataIterFree((DataIterHandle) handlePtr);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterBeforeFirst
  (JNIEnv *env, jobject obj, jobject handle) {
  jlong handlePtr = getLongField(env, handle);
  int ret = MXDataIterBeforeFirst((DataIterHandle) handlePtr);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterNext
  (JNIEnv *env, jobject obj, jobject handle, jobject out) {
  jlong handlePtr = getLongField(env, handle);
  int cout;
  int ret = MXDataIterNext((DataIterHandle)handlePtr, &cout);
  setIntField(env, out, cout);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetLabel
  (JNIEnv *env, jobject obj, jobject handle, jobject ndArrayHandle) {
  jlong handlePtr = getLongField(env, handle);
  NDArrayHandle out;
  int ret = MXDataIterGetLabel((DataIterHandle)handlePtr, &out);
  jclass refLongClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jfieldID refLongFid = env->GetFieldID(refLongClass, "value", "J");
  env->SetLongField(ndArrayHandle, refLongFid, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetData
  (JNIEnv *env, jobject obj, jobject handle, jobject ndArrayHandle) {
  jlong handlePtr = getLongField(env, handle);
  NDArrayHandle out;
  int ret = MXDataIterGetData((DataIterHandle)handlePtr, &out);
  jclass refLongClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jfieldID refLongFid = env->GetFieldID(refLongClass, "value", "J");
  env->SetLongField(ndArrayHandle, refLongFid, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetIndex
  (JNIEnv *env, jobject obj, jobject handle, jobject outIndex, jobject outSize) {
  jlong handlePtr = getLongField(env, handle);
  uint64_t* coutIndex;
  uint64_t coutSize;
  int ret = MXDataIterGetIndex((DataIterHandle)handlePtr, &coutIndex, &coutSize);
  //set field
  setLongField(env, outSize, (long)coutSize);
  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

//  printf("outSize: %ld\n", coutSize);
  for(int i=0; i<coutSize; i++) {
//    printf("%ld\t", coutIndex[i]);
    env->CallObjectMethod(outIndex, listAppend, (jlong)coutIndex[i]);
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetPadNum
  (JNIEnv *env, jobject obj, jobject handle, jobject pad) {
  jlong handlePtr = getLongField(env, handle);
  int cpad;
  int ret = MXDataIterGetPadNum((DataIterHandle)handlePtr, &cpad);
  setIntField(env, pad, cpad);
  return ret;
}