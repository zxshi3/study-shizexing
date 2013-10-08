/*
 * test for invoke java virtual machine from native code (c++)
 */
#include <jni.h>
#include <stdlib.h>

void checkException(JNIEnv* env) {
	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
	}
}

int main(int argc, char** argv) {
    JNIEnv *env;
    JavaVM *jvm;
    JavaVMInitArgs vm_args;
    jint res;
    jclass clzString = 0;
    jmethodID mid;
    jstring jstr;
    jobjectArray args;


    vm_args.version = JNI_VERSION_1_6;

    JNI_GetDefaultJavaVMInitArgs(&vm_args);

    res = JNI_CreateJavaVM(&jvm, (void**)&env,&vm_args);
    if (res < 0) {
        fprintf(stderr, "Can't create Java VM\n");
        exit(1);
    }


    clzString = env->FindClass("java/lang/String");
    if (clzString == 0) {
        fprintf(stderr, "Can't find String class\n");
        exit(1);
    } else {
        printf("clzString = %p\n", clzString);
    }

    checkException(env);
 
    // mid = (*env)->GetStaticMethodID(env, clzString, "main", "([Ljava/lang/String;)V");
    // if (mid == 0) {
        // fprintf(stderr, "Can't find Prog.main\n");
        // exit(1);
    // }

    // jstr = (*env)->NewStringUTF(env, " from C!");
    // if (jstr == 0) {
        // fprintf(stderr, "Out of memory\n");
        // exit(1);
    // }
    // args = (*env)->NewObjectArray(env, 1, 
                        // (*env)->FindClass(env, "java/lang/String"), jstr);
    // if (args == 0) {
        // fprintf(stderr, "Out of memory\n");
        // exit(1);
    // }
    // (*env)->CallStaticVoidMethod(env, cls, mid, args);

    jvm->DestroyJavaVM();
	
	return 0;
}

