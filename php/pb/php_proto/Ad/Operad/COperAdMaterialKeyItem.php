<?php
/**
 * Auto generated from operad_material.proto at 2015-12-04 16:27:42
 *
 * ad.operad package
 */

namespace Ad\Operad {
/**
 * COperAdMaterialKeyItem message
 */
class COperAdMaterialKeyItem extends \ProtobufMessage
{
    /* Field index constants */
    const DWCOMMODITYID = 1;
    const DWMATERIALID = 2;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::DWCOMMODITYID => array(
            'name' => 'dwCommodityId',
            'required' => true,
            'type' => 5,
        ),
        self::DWMATERIALID => array(
            'name' => 'dwMaterialId',
            'required' => true,
            'type' => 5,
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
        $this->values[self::DWCOMMODITYID] = null;
        $this->values[self::DWMATERIALID] = null;
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
     * Sets value of 'dwCommodityId' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setDwCommodityId($value)
    {
        return $this->set(self::DWCOMMODITYID, $value);
    }

    /**
     * Returns value of 'dwCommodityId' property
     *
     * @return int
     */
    public function getDwCommodityId()
    {
        return $this->get(self::DWCOMMODITYID);
    }

    /**
     * Sets value of 'dwMaterialId' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setDwMaterialId($value)
    {
        return $this->set(self::DWMATERIALID, $value);
    }

    /**
     * Returns value of 'dwMaterialId' property
     *
     * @return int
     */
    public function getDwMaterialId()
    {
        return $this->get(self::DWMATERIALID);
    }
}
}