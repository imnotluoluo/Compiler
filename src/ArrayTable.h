/**
 * @file IDTable.h
 * @brief 符号表的声明。定义见IDTable.cpp
 * @author Rhein_E
 * @version 1.0
 * @date 2023-03-28
 * 
 * @details 以栈的方式组织多个符号表，每个符号表对应一个域。
 * 使用时只需要创建一个IDTable类。完整的示例见文件末尾。
 */
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <vector>

#include "IR.h"

namespace sysy {

class AEntry {
public:
	Value *ptr;         ///< 数组首地址
    InitList *value;    ///< 数组值（类名字叫初值，实际上可不是初值()）
	// int ValType; // 0 is constantVal, 1 is varVal
	// //int DataType; //0 is int, 1 is float;
	std::vector<Value*> dims; // dim info

private:
    void check(InitList *cur, int depth) const {
        assert(depth < dims.size());
        assert(cur->getNumOperands() < dims[depth]);
        for (auto use : cur->getOperands()) {
            auto *child = use.getValue();
            if (child->getType() == Type::getInitListType()) {
                check(dynamic_cast<InitList *>(child), depth + 1);
            }
        }
    }

public:
	AEntry() = default;
	AEntry(Value *_ptr, InitList* _value, std::vector<Value*> &_dims):ptr(_ptr), value(_value), dims(_dims)
	{
        // 检查是否越界
        check(value, 0);

		// dims.clear();
		// std::cout<<"Init a AEntry!"<<std::endl;
		// int L = dim.size();
		// std::cout<<"L: "<<L<<std::endl;
		// for(int i=0;i<L;i++)
		// {
		// 	std::cout<<"i: "<<i<<std::endl;
		// 	dims.push_back(dim[i]);
		// }
		// int Len = data.size();
		// for(int i=0;i<Len;i++)
		// {
		// 	idata.push_back(data[i]);
		// }
		std::cout<<"Init Finish!"<<std::endl;
	};
	
}; // class AEntry

class ArrayTable {
using SubTable2=std::map<std::string, AEntry>;
private:
    std::vector<SubTable2 *> table;

public:
    /**
     * @brief 插入符号
     * @param[in] name 符号名字
     * @return true 插入成功 \\
     * @return false 插入失败，通常表明有名字冲突
     * @author Rhein_E
     * @example table.insert("x", (sysy::Value *) 0x1000080);
     */
    bool insert(const std::string &name, AEntry ptr);

    /**
     * @brief  查询符号
     * @param[in] name 符号的名字
     * @return 返回一个指向Value对象的指针，若为nullptr表示没有找到对应名字的符号
     * @author Rhein_E
     */
    AEntry *query(const std::string &name);

    /**
     * @brief 创建一个子表并入栈
     */
    void newTable();

    /**
     * @brief 销毁栈顶符号表
     */
    void destroyTop();

    /**
     * @brief 打印符号表（调试使用）
     * @author Rhein_E
     */
    void view() const;
};

} // namespace sysy

/*
#include"IDTable.h"
#include "IR.h"
int main() {
    sysy::IDTable table;

    // 一级符号表（全局）
    table.newTable(); 
    table.insert("x", (sysy::Value *) 0x1000080);
    table.insert("func", (sysy::Value *) 0x4000068);

    // 二级符号表（函数）
    table.newTable();
    table.insert("abc", (sysy::Value *) 0x8000040);
    table.insert("w", (sysy:: Value *) 0x8000008);
    
    table.view();

    // 查询符号表
    auto *ptr = table.query("abc");
    std::cout << "The pointer of \"abc\" is : ";
    std::cout << std::hex << std::showbase << (unsigned long long)ptr << "\n";

    return 0;
}
*/
