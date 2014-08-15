using System.Xml.Xsl;

public class Genstructs
	{
	public static void Main(string[] args)
		{
		var myXslTrans = new XslCompiledTransform(); 
		myXslTrans.Load("structs.xsl"); 
		myXslTrans.Transform("dataformat.xml","structs.h"); 
		}
	}
