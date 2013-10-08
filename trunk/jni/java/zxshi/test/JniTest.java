package zxshi.test;

public class JniTest {
	public JniTest() {
	}

	class Inner {
		public void print() {
			System.out.println("hello inner class");
		}
		public int get() {
			return i;
		}
	}

	public void setI(int i) {
		this.i = i;
	}

	public int getI() {
		return i;
	}

	public void setString(String str) {
		this.str = str;
	}

	public String getString() {
		return str;
	}

	@Override
	public String toString() {
		return new String("i = " + i + ", str = " + str);
	}

	private int i;
	private String str;
}

