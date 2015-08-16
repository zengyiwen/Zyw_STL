#pragma once


	struct __true_type
	{
	};

	struct __false_type
	{
	};

	template <class type>
	struct __type_traits
	{
		// ��Ҫ�Ƴ������Ա
		// ��֪ͨ���Զ��ػ�__type_traits�ı�����, �������__type_traits template���ػ���
		// ����Ϊ��ȷ����һ������ʹ����__type_traits����˴����κι�����ģ��ʱ
		// һ��Ҳ��˳������
		typedef __true_type     this_dummy_member_must_be_first;

		// ��������Ӧ��������, ��Ϊ�������п����Զ��������͵��ػ��汾
		//   - ��������°��ŵĳ�Ա����
		//   - ������Ƴ������Ƴ��ĳ�Ա
		//   - һ���������޸����г�Ա����, ȴû���޸ı������е���Ӧ����
		//   - �¼���ĳ�Ա������һ���Ա, ���Ǳ������ṩ����֧��

		typedef __false_type    has_trivial_default_constructor;
		typedef __false_type    has_trivial_copy_constructor;
		typedef __false_type    has_trivial_assignment_operator;
		typedef __false_type    has_trivial_destructor;
		typedef __false_type    is_POD_type;
	};


	// �������C++���õĻ������������ṩ�ػ��汾, ʹ�����trivial default constructor,
	// copy constructor, assignment operator, destructor
	// �������ΪPOD����
	//
	// �ػ�����:
	//         char, signed char, unsigned char,
	//         short, unsigned short
	//         int, unsigned int
	//         long, unsigned long
	//         float, double, long double

	// Provide some specializations.  This is harmless for compilers that
	//  have built-in __types_traits support, and essential for compilers
	//  that don't.
	template<>
	struct __type_traits<char>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<signed char>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<unsigned char>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<short>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<unsigned short>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<int>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<unsigned int>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<long>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<unsigned long>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<float>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<double>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<long double>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};


	// ���ָ���ṩ�ػ�
	template <class T>
	struct __type_traits<T*>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

	/* __STL_CLASS_PARTIAL_SPECIALIZATION */

	// ���char *, signed char *, unsigned char *�ṩ�ػ�
	template<>
	struct __type_traits<char*>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<signed char*>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};
	template<>
	struct __type_traits<unsigned char*>
	{
		typedef __true_type    has_trivial_default_constructor;
		typedef __true_type    has_trivial_copy_constructor;
		typedef __true_type    has_trivial_assignment_operator;
		typedef __true_type    has_trivial_destructor;
		typedef __true_type    is_POD_type;
	};

