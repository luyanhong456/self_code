<?php
/**
 * Auto generated from operad_material.proto at 2015-12-04 16:27:42
 *
 * ad.operad package
 */

namespace Ad\Operad {
/**
 * COperAdDelMaterialByKeyReq message
 */
class COperAdDelMaterialByKeyReq extends \ProtobufMessage
{
    /* Field index constants */
    const VMATERIALKEY = 1;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::VMATERIALKEY => array(
            'name' => 'vMaterialKey',
            'repeated' => true,
            'type' => '\Ad\Operad\COperAdMaterialKeyItem'
        ),
    );

    /**
     * Constructs new message container and clears its internal state
     *
     * @return null
     */
    public function __construct()
    {
        $this->reset();
    }

    /**
     * Clears message values and sets default ones
     *
     * @return null
     */
    public function reset()
    {
        $this->values[self::VMATERIALKEY] = array();
    }

    /**
     * Returns field descriptors
     *
     * @return array
     */
    public function fields()
    {
        return self::$fields;
    }

    /**
     * Appends value to 'vMaterialKey' list
     *
     * @param \Ad\Operad\COperAdMaterialKeyItem $value Value to append
     *
     * @return null
     */
    public function appendVMaterialKey(\Ad\Operad\COperAdMaterialKeyItem $value)
    {
        return $this->append(self::VMATERIALKEY, $value);
    }

    /**
     * Clears 'vMaterialKey' list
     *
     * @return null
     */
    public function clearVMaterialKey()
    {
        return $this->clear(self::VMATERIALKEY);
    }

    /**
     * Returns 'vMaterialKey' list
     *
     * @return \Ad\Operad\COperAdMaterialKeyItem[]
     */
    public function getVMaterialKey()
    {
        return $this->get(self::VMATERIALKEY);
    }

    /**
     * Returns 'vMaterialKey' iterator
     *
     * @return ArrayIterator
     */
    public function getVMaterialKeyIterator()
    {
        return new \ArrayIterator($this->get(self::VMATERIALKEY));
    }

    /**
     * Returns element from 'vMaterialKey' list at given offset
     *
     * @param int $offset Position in list
     *
     * @return \Ad\Operad\COperAdMaterialKeyItem
     */
    public function getVMaterialKeyAt($offset)
    {
        return $this->get(self::VMATERIALKEY, $offset);
    }

    /**
     * Returns count of 'vMaterialKey' list
     *
     * @return int
     */
    public function getVMaterialKeyCount()
    {
        return $this->count(self::VMATERIALKEY);
    }
}
}