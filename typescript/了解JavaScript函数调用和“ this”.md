### 了解JavaScript函数调用和“ this”

> 本文参考自：Understanding JavaScript Function Invocation and "this"，作者是：Yehuda Katz
>
> 多年来，我已经看到很多关于JavaScript函数调用的困惑。特别是，许多人抱怨`this`函数调用中的语义令人困惑。
>
> 在我看来，通过理解核心函数调用原语，然后在该原语之上考虑以其他方式调用功能作为糖，可以消除许多此类混淆。实际上，这正是ECMAScript规范对此的看法。在某些方面，此文章是对规范的简化，但基本思想是相同的。

#### 核心原语（The Core Primitive）

```javascript
function hello(thing) {
  console.log(this + " says hello " + thing);
}

hello.call("Yehuda", "world") 
//=> Yehuda says hello world
```

- 假设`argList`参数1到参数n的列表
- 这个`argList`的第一个值就是`this`的值

正如你看到的，我们调用`hello`，把`this`的值设置为`Yehuda`，并且附带一个参数值”world“。这就是JS函数的调用原语。

你可以认为其他方法的调用，去掉语法糖后，都是原语这种形式

> 语法糖：更简单易懂的表达式，但效果和原语一样

#### 简单函数调用

```typescript
function hello(thing) {
  console.log("Hello " + thing);
}
// 普通调用
hello("world")
// 脱糖调用
hello.call(window, "world");

// 这两种调用方式是等价的
```

#### 成员函数

函数作为实例中的成员被调用是另一种常见的嗲用方式，例如`person.hello()`

```typescript
var person = {
  name: "Brendan Eich",
  hello: function(thing) {
    console.log(this + " says hello " + thing);
  }
}

// this:
person.hello("world") //=> [object Object] says hello world 

// desugars to this:
person.hello.call(person, "world"); //=> [object Object] says hello world
```

注意，`hello`方法如何附加到对象中并不重要。我们将`hello`定义为一个独立的函数。让我们看看如果我们动态地将`hello`附加到对象上会发生什么:

```typescript
function hello(thing) {
  console.log(this + " says hello " + thing);
}

person = { name: "Brendan Eich" }
person.hello = hello;

person.hello("world") //=》 [object Object] says hello world

hello("world") //=》 [object global] says hello world
```

可以看出，这个`hello`函数没有一个固定的`this`概念。它总是根据调用者调用它的方式在调用时设置。

怎么解决这个问题呢，咱们接着向下看

#### 使用`Function.prototype.bind`

因为有时使用具有持久this值的函数引用会很方便，所以人们一直使用简单的闭包技巧将函数转换为具有不变`this`值的函数

```javascript
var person = {
  name: "Brendan Eich",
  hello: function(thing) {
    console.log(this.name + " says hello " + thing);
  }
}
var boundHello = function(thing) { return person.hello.call(person, thing); }
boundHello("world");
```

尽管我们调用`boundHello`仍然会调用`boundHello.call(window, "world")`，但是，我们通过原语使 `this`变回我们想要的值。

稍加修改，这个技巧会更通用

```js
var person = {
  name: "Brendan Eich",
  hello: function(thing) {
    console.log(this.name + " says hello " + thing);
  }
}

var bind = function(func, thisValue) {
  return function() {
    return func.apply(thisValue, arguments);
  }
}

var boundHello = bind(person.hello, person);
boundHello("world") // "Brendan Eich says hello world"
```

为了理解这一点，你只需要另外两条信息。

- 首先，`arguments`是一个类似数组的对象，它表示传递给函数的所有参数。

- 其次，该`apply`方法的工作方式与`call`原始方法完全相同，不同之处在于，该方法采用类似Array的对象，而不是一次列出一个参数。

我们的`bind`方法只是返回一个新函数。调用它时，我们的新函数仅调用传入的原始函数，将原始值设置为`this`。它还通过参数传递。

因为这是一个比较常见的习惯用法，所以ES5`bind`在所有`Function`实现此行为的对象上引入了一种新方法：

```js
var person = {
  name: "Brendan Eich",
  hello: function(thing) {
    console.log(this.name + " says hello " + thing);
  }
}

var bind = function(func, thisValue) {
  return function() {
    return func.apply(thisValue, arguments);
  }
}

var boundHello = person.hello.bind(person);
boundHello("world") //=> "Brendan Eich says hello world"
```

#### 在JQuery中

由于jQuery大量使用匿名回调函数，因此它在`call`内部使用方法将`this`这些回调的值设置为更有用的值。例如，jQuery不是`window`像`this`在所有事件处理程序中那样接收（就像没有特殊干预那样），而是jQuery`call`使用将事件处理程序设置为其第一个参数的元素来调用回调。

这是非常有用的，因为默认值`this`的匿名回调不是特别有用，但它可以给JavaScript能给初学者的印象`this`是，总的来说，这是一个奇怪的、经常发生变化的概念，很难进行推理。

如果您了解将含糖函数调用转换为脱糖函数的基本规则`func.call(thisValue, ...args)`，则应该能够浏览JavaScript`this`值中那些不太危险的地方。

