### typescript 中的 interface 和 type 到底有什么区别？

type:其作用就是给类型起一个新名字，可以作用于原始值（基本类型），联合类型，元组以及其它任何你需要手写的类型

#### 相同点

###### 都可以描述一个对象或者函数

```typescript
// interface
interface User {
  name: string
  age: number
}
interface SetUser {
  (name: string, age: number): void;
}

// type
type User = {
  name: string
  age: number
};
type SetUser = (name: string, age: number): void;

```

#### 拓展（extends）与 交叉类型（Intersection Types）

interface 可以 extends， 但 type 是不允许 extends 和 implement 的，**但是 type 缺可以通过交叉类型 实现 interface 的 extend 行为**，并且两者并不是相互独立的，也就是说 interface 可以 extends type, type 也可以 与 interface 类型 交叉 。

**虽然效果差不多，但是两者语法不同**。

#### interface extends interface

```
interface Name { 
  name: string; 
}
interface User extends Name { 
  age: number; 
}
```

#### type 与 type 交叉

```
type Name = { 
  name: string; 
}
type User = Name & { age: number };
```

#### interface extends type

```
type Name = {
  name: string;
}
interface User extends Name {
  age: number;
}
```

#### type 与 interface 交叉

```
interface Name {
  name: string;
}
type User = Name & {
  age: number;
}
```