# CModel
实现常见的PHP框架中所使用的CURD操作 实现类似于CI、TP 此类框架的模型库
##Requirement
- PHP 5.2+

## Install
```
$phpize;
$configure --with-php-config=/path/to/php-config
$make && make install
```
## demo
```php
<?php 
  $model = new CModel('table');  
  echo $model->table('new table')->select('id,name,datetime')->where('id = 1')->getSql();
?>
```
