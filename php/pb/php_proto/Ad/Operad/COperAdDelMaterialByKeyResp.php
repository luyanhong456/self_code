<?php
/**
 * Auto generated from operad_material.proto at 2015-12-04 16:27:42
 *
 * ad.operad package
 */

namespace Ad\Operad {
/**
 * COperAdDelMaterialByKeyResp message
 */
class COperAdDelMaterialByKeyResp extends \ProtobufMessage
{
    /* Field index constants */
    const VDELMATERIALKEY = 1;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::VDELMATERIALKEY => array(
            'name' => 'vDelMaterialKey',
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
        $this->values[self::VDELMATERIALKEY] = array();
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
     * Appends value to 'vDelMaterialKey' list
     *
     * @param \Ad\Operad\COperAdMaterialKeyItem $value Value to append
     *
     * @return null
     */
    public function appendVDelMaterialKey(\Ad\Operad\COperAdMaterialKeyItem $value)
    {
        return $this->append(self::VDELMATERIALKEY, $value);
    }

    /**
     * Clears 'vDelMaterialKey' list
     *
     * @return null
     */
    public function clearVDelMaterialKey()
    {
        return $this->clear(self::VDELMATERIALKEY);
    }

    /**
     * Returns 'vDelMaterialKey' list
     *
     * @return \Ad\Operad\COperAdMaterialKeyItem[]
     */
    public function getVDelMaterialKey()
    {
        return $this->get(self::VDELMATERIALKEY);
    }

    /**
     * Returns 'vDelMaterialKey' iterator
     *
     * @return ArrayIterator
     */
    public function getVDelMaterialKeyIterator()
    {
        return new \ArrayIterator($this->get(self::VDELMATERIALKEY));
    }

    /**
     * Returns element from 'vDelMaterialKey' list at given offset
     *
     * @param int $offset Position in list
     *
     * @return \Ad\Operad\COperAdMaterialKeyItem
     */
    public function getVDelMaterialKeyAt($offset)
    {
        return $this->get(self::VDELMATERIALKEY, $offset);
    }

    /**
     * Returns count of 'vDelMaterialKey' list
     *
     * @return int
     */
    public function getVDelMaterialKeyCount()
    {
        return $this->count(self::VDELMATERIALKEY);
    }
}
}