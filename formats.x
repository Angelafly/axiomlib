������� input-������ ��� axiomlib.

22 ������� 2006 ����.
----------------------------------


  ���������������� ����
  ---------------------
    BaseDataSetDir "/path/to/data/set/dir"
    DataSet "dataset_name"

    AxiomBaseDir "/path/to/axiom/base"

    AxiomSetBaseDir "/path/to/axiom/sets"
    AxiomSet "axiom_set_name1"
    AxiomSet "axiom_set_name2"

    Mode "expert"

  ����� ������
  ------------
  
    ����� ������ ������� ��:
      * ��������������
          - ������ ������� ��������� ��������
              ��� ������ - �������� (string, ����� ���� ������ �������)
      * ��������� ������� (��������� ����)
          - ��� ������ ��������� �������� - ��������� ���� (�� ����� ���� ���������)
          - ��������� ���� ����������� ��������� - �� ���� ����� ���� ���������
      * �������� ������� (�� ����� ���� ���������, � ������ - ��������� �����)
          - �������� ���������� �������� �� ������� �������� � ���������� ���������
      
    ����� ������ ���������� � ���������� ����������.
    ��������� ����������:
        /dataset			������
	    class_names			�������� ������� ����� ","
	    param_names			�������� ���������� (��� ����� ���� "time", � ����� � �� ����)
	    /reference			��������� ������� (��������� ����)
	       /class_name1		������ ��� ������ 1
	           run1.csv		���������� 1 ��� ������ 1
		   ...
	           runK.csv		���������� K ��� ������ 1
	       ...
	       /class_nameN		������ ��� ������ N
           /testing
	       run1.csv			������ �������� ������
	       run1-ref.csv		��������� �������� ������� ������� �� ������� ������� � ��������� ��������
	       ...
	       runP.csv			P--� �������� ������
	       runP-ref.csv		��������� �������� P-�� ������� �� ������� ������� � ��������� ��������

    ������ csv-������:

        time; param1; param2; param3
        0; 123.45; .34; 0.37
        1.5;123;.34;0.37;
        2.5; 0.0; .0; 00

    // ����������� ��������� ��� (unstructured)
    // ��� ��������
    class MultiTS {
      vector<vector<double> > data; // �� ����� paramNames
      vector<bool> validParams; // validParams[i]=0, ���� i-� �������� ���������� � ����, =1 �����
//      int ref_id; // ���������� �������������
      
      bool getTSByIndex(vector<double>*, int i); // =1 ���� ��� ok; =0 ���� ����� ��������� ���; i - ����� ���������
    }
    
    // ����� ��������� ��������� �����
    // ������ ������ - ����� ������ �� ���������� ������ (��. classNames)
    typedef vector<ClassTS> ReferenceClassesTS;
    
    // ����� ��������� ��������� ����� ��� ������ ������
    typedef vector<MultiTS> ClassTS;
    
    ���� ������
    -----------
    
        ������� ������� �� ������������ �������. ���� �� ������������� ������ ���������� 2 ������������
	�������.
	������� �������� ����������� ���������. ��� ��������� � ������ ������ ���� ������.
	
	���� ������ �������� � ��������� ��������:
	  /axiom_base1
	    name.x			�������� ���� ������
	    name1.ax			���� �������
	    name2.ax			���� �������
	    name3.ax			���� �������
	
	������ ����� ������:
	  �������� ������� (������ ��������� � ������ �����)
	  ������������ ������� 1 AND ������������ ������� 2
        ���	
	  �������� ������� (������ ��������� � ������ �����)
	  ������������ ������� 1


    ������� ������
    --------------

        name.as				������� ������
	
	������ ����� ������� ������:
	
        �������� ������� ������ (������ ��������� � ������ �����)
	�������� ������������ ���� ������
	1: �������� ������� 1
	    EC1: "param name" -> "param value"
	    EC1: "param name 2" -> "param value 2"
	    ...
	    EC1: "param name N" -> "param value N"
	    EC2: "param name" -> "param value"
	    EC2: "param name 2" -> "param value 2"
	    ...
	    EC2: "param name M" -> "param value M"
	...
	N: �������� ������� N
	
	