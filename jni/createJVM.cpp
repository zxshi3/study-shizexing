/**
 * Test for create jvm from native code
 *
 * javap -s package.class to get the signature
 */

#include <stdio.h>
#include <jni.h>
#include <string>
#include <stdlib.h>

JNIEnv* create_vm(JavaVM** ppJvm) {
	JNIEnv* env;
	JavaVMInitArgs args;
	JavaVMOption options[1];
	//std::string classpath = "-Djava.class.path=.:./java:/home/telenav/tests/jni/java";
	std::string classpath = "-Djava.class.path=.:./java";
	options[0].optionString = const_cast<char*>(classpath.c_str());
	
	args.version = JNI_VERSION_1_6;
	args.nOptions = 1;
	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;

	int r = JNI_OK;
	//r = JNI_GetDefaultJavaVMInitArgs(&args);
	if (JNI_OK != r)
	{
		fprintf(stderr, "JNI_GetDefaultJavaVMInitArgs return error %d\n", r);
	}
	r = JNI_CreateJavaVM(ppJvm, (void **)&env, &args);
	if (JNI_OK != r)
	{
		fprintf(stderr, "JNI_CreateJavaVM return error %d\n", r);
	}
	printf("pJVM = %p, env = %p\n", *ppJvm, env);


	{
	jclass clzString = env->FindClass("java/lang/String");
	if (clzString == 0) {
		fprintf(stderr, "Can't find String class\n");
		exit(1);
	} else {
		printf("clzString = %p\n", clzString);
	}
	}

	// Attach my self ?
	JavaVMAttachArgs targs;
	jint ret = (*ppJvm)->AttachCurrentThread((void**)&env, &targs);
	printf("ret = %d, pJVM = %p, env = %p\n", ret, *ppJvm, env);
	return env;
}

void checkException(JNIEnv* env) {
	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
	}
}

static jclass clzJniTest = 0;
static jclass clzString = 0;
void initJavaReference(JNIEnv* env) {
	if (clzJniTest == 0) {
		clzJniTest = env->FindClass("zxshi/test/JniTest");
		printf("clzJniTest = %p\n", clzJniTest);
		checkException(env);
		if (clzJniTest != 0) {
			clzJniTest = (jclass)env->NewGlobalRef(clzJniTest);
			printf("clzJniTest = %p\n", clzJniTest);
			checkException(env);
		}
	}
	if (clzString == 0) {
		clzString = env->FindClass("java/lang/String");
		printf("clzString = %p\n", clzString);
		if (clzString != 0) {
			clzString = (jclass)env->NewGlobalRef(clzString);
			printf("clzString = %p\n", clzString);
		}
	}

}

int main(int argc, char **argv) {
	JavaVM* pJVM = 0;
	JNIEnv* env = create_vm(&pJVM);
	jint ver = env->GetVersion();
	printf("version is : %d\n", ver);

	initJavaReference(env);

	// check for exceptions
	checkException(env);

	// destroy 
	pJVM->DestroyJavaVM();
	pJVM = 0;
}

