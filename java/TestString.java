
public class TestString {
    private static final String oldHost = "tweddletech.com";
    private static final String newHost = "api.datlconnect.com";
	private static String a = "https://api.datlconnect.com/1.0/softwareupdate?currentVersion=0.0.0&HUType=910.1.L0800480130NA0&entune=1";
	private static String b = "https://api.datlconnect.com/1.0/softwareupdate?entune=1&currentVersion=&HUType=910.1.L0800480130NA0";
	private static String c = "https://dev-api.tweddletech.com/1.0/softwareupdate?entune=1&currentVersion&HUType=910.1.L0800480130NA0";
	private static String redirectHost = "dev-api.datlconnect.com";
	
	public void testRedirect(String s) {
		System.out.println(s);
		if (s.contains(oldHost) || s.contains(newHost)) {
			boolean hasFirstParam = false;
			StringBuilder sb = new StringBuilder();
			sb.append("https://").append(redirectHost).append("/1.0/softwareupdate");
			if (s.contains(oldHost)) {
				hasFirstParam = true;
				sb.append("?entune=1");
			}
			int idxcurrentVersion = s.indexOf("currentVersion");
			if (idxcurrentVersion != -1) {
				int idxcurrentVersion2 = s.indexOf("&", idxcurrentVersion);
				if (hasFirstParam == true) {
					sb.append("&");
				} else {
					sb.append("?");
					hasFirstParam = true;
				}
				if (idxcurrentVersion2 == -1) {
					sb.append(s.substring(idxcurrentVersion));
				} else {
					sb.append(s.substring(idxcurrentVersion, idxcurrentVersion2));
				}
			}
			int idxHUType = s.indexOf("HUType");
			if (idxHUType != -1) {
				int idxHUType2 = s.indexOf("&", idxHUType);
				if (hasFirstParam == true) {
					sb.append("&");
				} else {
					sb.append("?");
					hasFirstParam = true;
				}
				if (idxHUType2 == -1) {
					sb.append(s.substring(idxHUType));
				} else {
					sb.append(s.substring(idxHUType, idxHUType2));
				}
			}
			System.out.println(sb.toString());
			System.out.println(sb.toString());
		}
	}
	
	public String[] split(String s, String d) {
		return s.split(d);
	}
	
	public void printStringArray(String [] ss) {
		System.out.println("ss.length = " + ss.length);
		for (String s : ss) {
			System.out.println("s = " + s);
		}
	}
	
	public void testSplit() {
		String s = "abc\ndef";
		System.out.println(s);
		printStringArray(split(s, "\n"));
		s = "\nabc\ndef";
		System.out.println(s);
		printStringArray(split(s, "\n"));
		s = "\nabc\ndef\n";
		System.out.println(s);
		printStringArray(split(s, "\n"));
		s = "\n\nabc\ndef\n";
		System.out.println(s);
		printStringArray(split(s, "\n"));
		s = "\n\nabc\n\n\ndef\n";
		System.out.println(s);
		printStringArray(split(s, "\n"));
		s = "\n\nabc\n\n\ndef\n\n";
		System.out.println(s);
		printStringArray(split(s, "\n"));
	}
    
    public void testReplace() {
        String s = "http://www.baidu.com";
        // this test is to make sure http: does not contain special character for regular expresion
        s = s.replaceFirst("http:", "https:");
        System.out.println(s);
    }
	
	public static void main(String[] args) {
		System.out.println("test string");
		TestString test = new TestString();
		test.testRedirect(a);
		test.testRedirect(b);
		test.testRedirect(c);
        
        test.testReplace();
		
		test.testSplit();
	}
}
